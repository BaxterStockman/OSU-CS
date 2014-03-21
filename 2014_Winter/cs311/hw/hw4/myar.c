/*
*  Name: Matt Schreiber
*  Email: schreibm@onid.oregonstate.edu
*  Class: CS311-400
*  Assignment: 4
*/

#include <ar.h>             /* for struct ar_hdr */
#include <assert.h>         /* for assert() */
#include <ctype.h>
#include <dirent.h>
#include <getopt.h>         /* for getopt_long() and associated entities */
#include <errno.h>
#include <libgen.h>         /* for basename() */
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

/* Naming convention due to Dan Albert
 * Macros for holding the sizes of the
 * various members of the ar_hdr struct */
#define SARFNAME    16
#define SARFDATE    12
#define SARFUID     6
#define SARFGID     6
#define SARFMODE    8
#define SARFSIZE    10
#define SARFFMAG    2

/* Total size of the ar_hdr struct */
#define SARFHDR     60

/* Format string for conversion from
 * stat() to ar_hdr struct. */
#define ARFHDRF "%-16s%-12lld%-6u%-6u%-8o%-10lld%-2s"

#define BLOCKSIZE 1024

/* Limit on string writing buffer  */
#define BUFSIZE 255

/* Default file permissions */
#define DFLTMODE S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
#define DFLTUMASK S_IWGRP | S_IWOTH

/* Enum type to help with switch statements
 * concerning ar_hdr member variables */
enum hdr_attr { name_attr, date_attr, uid_attr, gid_attr, mode_attr, size_attr, fmag_attr };

/* Due to GNU ar.c -- makes switch
 * statements with operation options easier */
enum operations
{
    none = 0,
    append,
    append_all,
    extract,
    contents,
    verbose,
    delete
} operation = none;

/* Struct for holding values converted
 * from the member variables of an
 * ar_hdr struct.  Simplifies printing
 * and other operations. */
struct
shadow_hdr
{
    char    ar_name[16];
    time_t  ar_date;
    uid_t   ar_uid;
    gid_t   ar_gid;
    mode_t  ar_mode;
    off_t   ar_size;
};

/* Print usage message */
static void
usage(FILE *stream, const char *prog_name, const char *err, int exit_code)
{
    fprintf(stream, "Error: %s\n", err);
    fprintf(stream, "Usage: %s key archive-file [options] [member [...]]\n",
                    prog_name);
    fprintf(stream,
            "   -q  --quick     Quickly append named files to archive\n"
            "   -x  --extract   Extract named members from the archive\n"
            "   -t  --contents  Print concise table of archive contents\n"
            "   -v  --verbose   Print verbose table of archive contents\n"
            "   -d  --delete    Delete named files from archive\n"
            "   -A  --append    Quickly append all regular files in\n"
            "                   current directory to archive \n"
            "   -w  --when N    Add all files in working directory modified\n"
            "                   in next N secondsto archive\n"
            );
    exit(exit_code);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  isdir
 *  Description:  Returns 1 if 'path' argument refers to a directory, 0 otherwise
 * =====================================================================================
 */
static inline int
isdir(const char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    return S_ISDIR(statbuf.st_mode);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  isempty
 *  Description:  Returns 1 if 'path' is a file of size 0, 0 otherwise
 * =====================================================================================
 */
static inline int
isempty(const char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    return(!statbuf.st_size);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  isempty_arch
 *  Description:  Returns 1 is 'path' refers to an ar-format archive that is
 *  empty except for the ARMAG header, 0 otherwise.
 * =====================================================================================
 */
static inline int
isempty_arch(const char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    return(statbuf.st_size == SARMAG);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  strmode
 *  Description:  Converts a mode_t value into a 9-character string
 *  representing the permissions on a given file.  The string is much like what
 *  you'd get from 'ls -l'.
 *
 *  Help in writing this function was found at:
 *  http://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
 * =====================================================================================
 */
static void
strmode(mode_t m, char *p) {
    *p++ = m & S_IRUSR ? 'r' : '-';
    *p++ = m & S_IWUSR ? 'w' : '-';
    *p++ = m & S_IXUSR ? 'x' : '-';
    *p++ = m & S_IRGRP ? 'r' : '-';
    *p++ = m & S_IWGRP ? 'w' : '-';
    *p++ = m & S_IXGRP ? 'x' : '-';
    *p++ = m & S_IROTH ? 'r' : '-';
    *p++ = m & S_IWOTH ? 'w' : '-';
    *p++ = m & S_IXOTH ? 'x' : '-';
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_modestr
 *  Description:  Calls strmode to print the 9-character permission string.
 * =====================================================================================
 */
static inline void
print_modestr(mode_t mode)
{
    char modestr[10];
    modestr[9] = '\0';
    strmode(mode, modestr);
    printf("mode: %s\n", modestr);

}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_file_size
 *  Description:  Returns the size in bytes of the file referred to by the file
 *  descriptor parameter.
 *
 *  Pre:  The file referred to by 'fd' is open
 * =====================================================================================
 */
static inline off_t
get_file_size(int fd)
{
    struct stat stat;
    fstat(fd, &stat);
    return stat.st_size;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_cur_offset
 *  Description:  Returns the current offset within the open file referred to
 *  by the file descriptor parameter.
 *
 *  Pre:  The file referred to by 'fd' is open
 * =====================================================================================
 */
static inline off_t
get_cur_off(int fd)
{
    return lseek(fd, 0, SEEK_CUR);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  write_armag
 *  Description:  Writes the ARMAG magic header string to the very beginning of
 *  the file referred to by the file descriptor parameter.
 *
 *  Pre:  The file referred to by 'fd' is open
 * =====================================================================================
 */
static inline off_t
write_armag(int fd)
{
    /* Seek to beginning of file */
    lseek(fd, 0, SEEK_SET);
    /* Attempt to write magic string */
    return (off_t)(write(fd, ARMAG, SARMAG) == -1);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  seek_past_armag
 *  Description:  Readss the first SARMAG bytes of an open file, comparing them
 *  to the ARMAG macro.  Returns the current offset if the match is successful,
 *  -1 otherwise.
 *
 *  Pre:  The file referred to by 'fd' is open
 * =====================================================================================
 */
static inline off_t
seek_past_armag(int fd)
{
    char s[SARMAG+1];

    if(lseek(fd, 0, SEEK_SET) == -1
        || read(fd, s, SARMAG) == -1) {
        perror("read");
        goto cleanup;
    } else if(strncmp(s, ARMAG, SARMAG) != 0) {
        fprintf(stderr, "file format not recognized\n");
        goto cleanup;
    } else {
        return(get_cur_off(fd));
    }

cleanup:
    if(close(fd) == -1) {
        perror("close");
        fprintf(stderr, "error closing file\n");
    }
    return -1;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_hdr_attr
 *  Description:  Copies a value of one ar_hdr member variable into a
 *  null-terminated buffer string.
 *
 *  Pre: 'buf' is of sufficient size to hold the attribute string, and 'buflen'
 *  is the number of characters in 'buf'
 * =====================================================================================
 */
static void
get_hdr_attr(const struct ar_hdr *hdr, enum hdr_attr attr, char *buf, size_t buflen)
{
    memset(buf, '\0', buflen);
    switch(attr) {
        case name_attr:
            strncpy(buf, hdr->ar_name, SARFNAME);
            char *c = strrchr(buf, '/');
            *c = '\0';
            break;
        case date_attr:
            strncpy(buf, hdr->ar_date, SARFDATE);
            break;
        case uid_attr:
            strncpy(buf, hdr->ar_uid, SARFUID);
            break;
        case gid_attr:
            strncpy(buf, hdr->ar_gid, SARFGID);
            break;
        case mode_attr:
            strncpy(buf, hdr->ar_mode, SARFMODE);
            break;
        case size_attr:
            strncpy(buf, hdr->ar_size, SARFSIZE);
            break;
        default:
            buf = NULL;
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_member_size
 *  Description:  Returns an off_t variable representing the size of the file
 *  represented in an ar_hdr struct.
 * =====================================================================================
 */
static inline off_t
get_member_size(struct ar_hdr *hdr)
{
    char buf[SARFSIZE+1];
    get_hdr_attr(hdr, size_attr, buf, SARFSIZE+1);

    return atoll(buf);
}

static inline void
print_member_name(struct ar_hdr *hdr)
{
    char buf[SARFNAME+1];
    get_hdr_attr(hdr, name_attr, buf, SARFSIZE+1);

    printf("PROCESSING %s\n", buf);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  fill_shadow_hdr
 *  Description:  Converts the string values in an ar_hdr struct into the
 *  various variable types in a shadow_hdr struct.
 * =====================================================================================
 */
/* Conversion specifications due to Dan Albert */
static void
fill_shadow_hdr(const struct ar_hdr *hdr, struct shadow_hdr *dest)
{
    size_t len = SARFNAME;
    char buf[len];

    get_hdr_attr(hdr, name_attr, buf, len);
    memcpy(dest->ar_name, buf, len);

    get_hdr_attr(hdr, date_attr, buf, len);
    dest->ar_date = strtol(buf, NULL, 10);

    get_hdr_attr(hdr, uid_attr, buf, len);
    dest->ar_uid = strtol(buf, NULL, 10);

    get_hdr_attr(hdr, gid_attr, buf, len);
    dest->ar_gid = strtol(buf, NULL, 10);

    /* Base in call to strtol is 8 because
     * the permission value is represented
     * in octal */
    get_hdr_attr(hdr, mode_attr, buf, len);
    dest->ar_mode = strtol(buf, NULL, 8);

    get_hdr_attr(hdr, size_attr, buf, len);
    dest->ar_size = strtol(buf, NULL, 10);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  get_hdr
 *  Description:  Fills a struct header with values converted read from an
 *  archive file referred to by the file descriptor parameter.
 *
 *  Pre:  The file referred to by 'fd' is open and in valid ar-style archive
 *  format, and 'offset' refers to a valid offset in the file.
 * =====================================================================================
 */
static off_t
get_hdr(int fd, off_t offset, int whence, struct ar_hdr *hdr)
{
    /* Seek to offset in fd from whence */
    if(lseek(fd, offset, whence) == -1) {
        perror("lseek");
        fprintf(stderr, "get_hdr: failed to seek to offset %lld\n",
                (long long)offset);
        return -1;
    }
    /* Read SARFHDR bytes into hdr
     * and return the bytes read cast as an offset */
    if(read(fd, hdr, SARFHDR) == -1) {
        fprintf(stderr, "get_hdr: failed to read %zu bytes at offset %lld\n",
                (size_t)SARFHDR, (long long)offset);
        return -1;
    }

    /* Return current offset, which should be
     * just after the header */
    return get_cur_off(fd);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  init_ar_hdr
 *  Description:  Returns an ar_hdr struct initialied with spaces, a forward
 *  slash in the first position of ar_name, and zeroes in the first position of
 *  everything else but ar_fmag, which contains the magic file header string.
 *
 *  This function is adapted from the arlib_init() function
 *  from Android's elfutils, available under the GNU GPLv2 at
 *  https://android.googlesource.com/platform/external/elfutils/+/25b3c049e70834cf33790a28643ab058b507b35c/0.153/src/arlib.c
 * =====================================================================================
 */
static struct ar_hdr
init_ar_hdr(void)
{
    struct ar_hdr hdr;

    /* Initialize entire header with spaces */
    memset(&hdr, 0x20, SARFHDR);

    /* Insert initializer characters */
    hdr.ar_name[0] = '/';
    hdr.ar_date[0] = '0';
    hdr.ar_uid[0] = '0';
    hdr.ar_gid[0] = '0';
    hdr.ar_mode[0] = '0';
    hdr.ar_size[0] = '0';

    /* Copy magic terminating string to hdr.ar_fmag */
    memcpy(hdr.ar_fmag, ARFMAG, SARFFMAG);

    return hdr;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  fill_ar_hdr
 *  Description:  Fills an ar_hdr struct with values converted from a stat()
 *  call on the file referred to by 'path'.
 *
 *  Pre: The file referred to by 'path' exists and can be read by stat().
 * =====================================================================================
 */
static void
fill_ar_hdr(char *path, struct ar_hdr *hdr)
{
    struct stat statbuf;
    char buf[SARFHDR+1];
    char *bname = basename(path);
    size_t bname_len = strlen(bname);

    /* Break if the basename isn't short
     * enough to fit in hdr->ar_name */
    assert(bname_len <= 15);

    *hdr = init_ar_hdr();

    if(stat(path, &statbuf) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    snprintf(buf, SARFHDR+1,
        ARFHDRF,
        bname,
        (long long)statbuf.st_mtime,
        statbuf.st_uid,
        statbuf.st_gid,
        statbuf.st_mode,
        (long long)statbuf.st_size,
        ARFMAG
    );

    buf[bname_len] = '/';

    memcpy(hdr, buf, SARFHDR);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  map_over_members
 *  Description:  Calls the function referred to by pointer 'func' on each
 *  member of an archive file.
 *
 *  Pre:  The file referred to by fd is open and has the necessary permissions
 *  and read/write attributes to perform 'func' on its contents.

 *  It's ugly, but it (mostly) works.  Once I had gotten down the basics of iterating
 *  over the archive file, I didn't want to have to repeat the logic separately for
 *  append, print, and delete.  That's why the function pointer is so darn hideous. I'm
 *  sure there's better way to do this, but I haven't learned it yet --- obviously :).
 * =====================================================================================
 */
static off_t
map_over_members(int fd, void (*func)(int, struct ar_hdr *, char **, int, int),
        char **files, int count, int temp_fd)
{
    struct ar_hdr hdr;
    off_t hdr_off, member_off, member_size;
    off_t ar_size = get_file_size(fd);

    /* First member header should be just
     * past archive magic header */
    if((hdr_off = seek_past_armag(fd)) == -1)
        return -1;

    while((member_off = get_hdr(fd, hdr_off, SEEK_SET, &hdr)) != -1) {

        /* Get member file's size in preparation
         * for next jump */
        member_size = get_member_size(&hdr);

        /* Here to help with diagnostics */
        /*
        print_member_name(&hdr);
        printf("member offset: %lld\n", (long long)member_off);
        printf("archive size: %lld\n", (long long)ar_size);
        */

        func(fd, &hdr, files, count, temp_fd);

        /* Make sure we're just before the member
         * file's contents */
        lseek(fd, member_off, SEEK_SET);

        /* Align jump to even bit size */
        member_size = member_size % 2 ? member_size + 1 : member_size;

        /* Set hdr_off to next header's offset;
         * exit loop if we're going to hit the
         * archive's boundary */
        if((hdr_off = member_off + member_size) >= ar_size)
            break;
    }

    /* Return to the beginning of the archive */
    return lseek(fd, 0, SEEK_SET);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  hdrcmp
 *  Description:  Compares certain parameters in two ar_hdr structs to one
 *  another, returning the value of one of two call to memcmp().
 *
 *  I have deemed archive file entries to be identical if they share name,
 *  date, uid, and size.
 * =====================================================================================
 */
static inline int
hdrcmp(const struct ar_hdr *hdr1, const struct ar_hdr *hdr2)
{
    int ret;
    if((ret = memcmp(hdr1, hdr2, SARFNAME + SARFDATE + SARFUID)) != 0) {
        return ret;
    } else {
        return memcmp(hdr1->ar_size, hdr2->ar_size, SARFSIZE);
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  match_fnames
 *  Description:  Iterates over an array of filenames, comparing them to the
 *  filename passed as the first parameter.  The comparison is based on the
 *  function 'compar'.
 *
 *  Pre:  'compar' must return 0 for matches (however that is defined), and any
 *  other integer value(s) for non-matches.  Intended mostly for use with the
 *  strcmp() function from <string.h>
 *
 *  Post: Returns the index in 'files' of the match, -1 otherwise.
 * =====================================================================================
 */
static int
match_fnames(char *fname, char **files, int count,
        int (*compar)(const char *, const char *))
{
    int i = 0;
    for(i = 0; i < count; i++) {
        if(files[i] != NULL)
            if(compar(fname, files[i]) == 0)
                return i;
    }

    return -1;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  compare_basename
 *  Description:  Compares the basename of the file referred to by 'path' to
 *  the string contained in 'fname'.  Returns the value of the strcmp() call.
 *
 *  Pre: 'path' refers to an existing file with the proper permissions, etc.,
 *  to call basename() on it.
 * =====================================================================================
 */
static int
compare_basename(const char *fname, const char *path)
{
    size_t len = strlen(path);
    char mod_path[len+1];
    strcpy(mod_path, path);

    char *bname = basename(mod_path);
    return(strcmp(fname, bname));
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  check_members
 *  Description:  Compares the name of the file held in an ar_hdr struct to the
 *  basenames of the files referred to by the filenames in 'files'.  If there
 *  is a match, generates an ar_hdr for the matched file and calls hdrcmp on it
 *  and the header 'hdr'.  If the headers match, replaces the matching filename
 *  in 'files' with NULL.
 *
 *  Pre: The filenames in 'files' refer to existing files with the necessary
 *  permissions, etc.
 * =====================================================================================
 */
static void
check_members(int fd, struct ar_hdr *hdr, char **files, int count, int temp_fd)
{
    char fname[SARFNAME+1];
    get_hdr_attr(hdr, name_attr, fname, SARFNAME+1);

    struct ar_hdr new_hdr;
    int match = match_fnames(fname, files, count, &compare_basename);
    if(match != -1) {
        fill_ar_hdr(files[match], &new_hdr);
        if(hdrcmp(hdr, &new_hdr) == 0) {
            files[match] = NULL;
        }
    }

    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  check_members
 *  Description:  Extracts a file from an ar-style archive.
 *
 *  Pre:  The file referred to by 'fd' exists and is readable.
 * =====================================================================================
 */
static void
extract_member(int fd, struct ar_hdr *hdr, char **files, int count, int temp_fd)
{
    off_t member_off = get_cur_off(fd); char fname[SARFNAME+1];
    get_hdr_attr(hdr, name_attr, fname, SARFNAME+1);


    int match = match_fnames(fname, files, count, &strcmp);

    /* Set the matching name to NULL so that
     * caller can figure out that a matching
     * archive member was found */
    if(match != -1)
        files[match] = NULL;

    /* Only enter loop if either (1)count
     * was <= 0 to begin with, or (2) we
     * found a matching filename */
    if(count <= 0 || (match != -1 && count > 0)) {
        struct shadow_hdr shadow;
        struct utimbuf tbuf;
        char write_buf[BLOCKSIZE];
        size_t size;
        int extract_fd;

        fill_shadow_hdr(hdr, &shadow);
        char *fname = shadow.ar_name;

        if((extract_fd = open(fname, O_CREAT | O_WRONLY | O_EXCL,
                        shadow.ar_mode)) == -1) {
            fprintf(stderr, "error opening file %s\n", fname);
            perror("open");
            return;
        }

        /* Write to file referred to by extract_fd
         * in blocks of bytes of size BLOCKSIZE*/
        size = shadow.ar_size;
        while(size > 0) {
            if(read(fd, write_buf, size) == -1) {
                fprintf(stderr, "error reading from file\n");
                perror("read");
                goto cleanup;
            }
            if(write(extract_fd, write_buf, size) == -1) {
                perror("write");
                fprintf(stderr, "error writing to file file %s\n", fname);
                goto cleanup;
            }
            size -= size < BLOCKSIZE ? size : BLOCKSIZE;
        }

        /* Set the atime and mtime of the
         * tbuf struct to the ar_date value
         * of the shadow_hdr struct */
        tbuf.actime = shadow.ar_date;
        tbuf.modtime = shadow.ar_date;
        utime(fname, &tbuf);

        /* Restore file permissions stored
         * in shadow.ar_uid and shadow.ar_gid */
        if(fchown(extract_fd, shadow.ar_uid, shadow.ar_gid) == -1) {
            perror("chown");
            fprintf(stderr, "failed to change ownership of file %s\n", fname);
        }

        lseek(fd, member_off, SEEK_SET);
        return;

cleanup:
        if(close(extract_fd) == -1) {
            perror("close");
            fprintf(stderr, "error closing archive file\n");
        }
        if(close(extract_fd) == -1) {
            perror("close");
            fprintf(stderr, "error closing file %s\n", fname);
        }
        if(unlink(fname) == -1) {
            perror("unlink");
            fprintf(stderr, "error deleting file %s\n", fname);
        }
        lseek(fd, member_off, SEEK_SET);
        return;
    }
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  check_members
 *  Description:  Wrapper for map_over_members called with extract_member as
 *  'func' parameter.  Prints an error message if any filename in 'files'
 *  couldn't be found in the archive.
 *
 *  Pre:  'fd' refers to a file that exists and has the necessary permissions,
 *  etc.
 * =====================================================================================
 */
static void
extract_files(int fd, char **files, int count)
{
    map_over_members(fd, extract_member, files, count, -1);

    int i;
    for(i = 0; i < count; i++)
        if(files[i] != NULL)
            fprintf(stderr, "no file %s in archive\n", files[i]);
}

static void
write_to_arch(int src, int dest, size_t to_write)
{
    char write_buf[BLOCKSIZE];
    size_t size = to_write;

    /* Write file contents to
     * temporary file */
    while(to_write > 0) {
        if(read(src, write_buf, to_write) == -1) {
            perror("read");
            fprintf(stderr, "error reading from file\n");
            goto cleanup;
        }
        if(write(dest, write_buf, to_write) == -1) {
            perror("write");
            fprintf(stderr, "error writing to archive file\n");
            goto cleanup;
        }
        to_write -= to_write < BLOCKSIZE ? to_write : BLOCKSIZE;
    }

    /* Add a newline if the file size
     * is an odd number */
    if(size % 2 != 0) {
        char c = '\n';
        if(write(dest, &c, 1) == -1) {
            perror("write");
            fprintf(stderr, "error writing to archive file\n");
            goto cleanup;
        }
    }

    return;

cleanup:
    if(close(src) == -1) {
        fprintf(stderr, "error closing source file\n");
        perror("close");
    }
    if(close(dest) == -1) {
        fprintf(stderr, "error closing destination file\n");
        perror("close");
    }
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  retain_member
 *  Description:  Function to be passed to map_over_members from delete_files.
 *  NULLs out a filename in 'files' and returns if there is a match to an
 *  archive member.  Otherwise, copies the ar_hdr struct and the subsequent
 *  file data to the temporary file referred to by 'temp_fd'.
 *
 *  Pre: 'temp_fd' refers to a file that exists and is writable; 'arch' refers
 *  to a file that exists and is readable.
 * =====================================================================================
 */
static void
retain_member(int arch, struct ar_hdr *hdr, char **files, int count, int temp_fd)
{
    char fname[SARFNAME+1];
    get_hdr_attr(hdr, name_attr, fname, SARFNAME+1);

    int match = match_fnames(fname, files, count, strcmp);

    /* If there was no match, set
     * files to NULL so that delete_files
     * can tell user that no file was found.
     * Otherwise, return so that we don't copy
     * the file. */
    if(match != -1) {
        files[match] = NULL;
        return;
    }

    /* Write header to temporary file */
    if(write(temp_fd, hdr, SARFHDR) == -1) {
        fprintf(stderr, "error writing to file\n");
        perror("write");
        goto cleanup;
    }

    size_t to_write = get_member_size(hdr);
    write_to_arch(arch, temp_fd, to_write);

    return;

cleanup:
    if(close(arch) == -1) {
        perror("close");
        fprintf(stderr, "error closing archive file\n");
    }
    if(close(temp_fd) == -1) {
        perror("close");
        fprintf(stderr, "error closing temporary file\n");
    }
    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  delete_files
 *  Description:  Wrapper for map_over_members and retain_file.  Creates a
 *  temporary archive file for holding the contents of all members of archive
 *  'archname' that do not match a filename in 'files'.  Once 'archname' is
 *  done being processed by map_over_members, unlinks it and renames the
 *  temporary file to 'archname'.
 *
 *  Pre:  'arch' and 'archname' refer to an archive file that is open and
 *  readable.
 * =====================================================================================
 */
static void
delete_files(int arch, char *archname, char **files, int count)
{
    int temp_fd;
    char tempname[] = "temp-archive-XXXXXX";

    if((temp_fd = mkstemp(tempname)) == -1) {
        perror("mkstemp");
        fprintf(stderr, "error creating temporary file\n");
        goto cleanup;
    }

    /* Write the ARMAG header to
     * the temporary file */
    if(isempty(tempname))
        write_armag(temp_fd);

    map_over_members(arch, &retain_member, files, count, temp_fd);

    int i;
    for(i = 0; i < count; i++)
        if(files[i] != NULL)
            fprintf(stderr, "no member %s in archive %s\n", files[i], archname);
    /*
    if(close(arch) == -1) {
        fprintf(stderr, "failed to close archive %s\n", archname);
        perror("close");
    } */

    if(unlink(archname) == -1) {
        fprintf(stderr, "failed to delete archive %s\n", archname);
        perror("unlink");
        goto cleanup;
    }

    if(rename(tempname, archname) == -1) {
        fprintf(stderr, "failed to rename temporary file %s\n", tempname);
        perror("rename");
        goto cleanup;
    }

    if(close(temp_fd) == -1) {
        fprintf(stderr, "failed to close temporary file %s\n", tempname);
        perror("close");
    }

    return;

cleanup:
    if(unlink(tempname) == -1) {
        fprintf(stderr, "failed to delete archive %s\n", tempname);
        perror("unlink");
        goto cleanup;
    }

    if(close(temp_fd) == -1) {
        fprintf(stderr, "failed to close temporary file %s\n", tempname);
        perror("close");
    }

    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  copy_file
 *  Description:
 *
 *  Thanks to user plinth at:
 *  http://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c
 * =====================================================================================
 */
void
copy_file(char *source, char *dest)
{
    int child_exit_status;
    pid_t pid;
    int status;
    if (!source || !dest) {
        fprintf(stderr, "no source/destination file specified");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == 0) {
        execl("/bin/cp", "/bin/cp", source, dest, (char *)0);
    } else if (pid < 0) {
        fprintf(stderr, "failed to start /bin/cp\n");
        exit(EXIT_FAILURE);
    } else {
        //pid_t ws = waitpid( pid, &child_exit_status, WNOHANG);
        pid_t ws = wait(&child_exit_status);
        if (ws == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }

        if( WIFEXITED(child_exit_status)) {
            status = WEXITSTATUS(child_exit_status);
            if(status !=0) {
                perror("exit");
                exit(status);
            }
        }
        else if (WIFSIGNALED(child_exit_status)) {
            fprintf(stderr, "child process was killed");
            //exit(EXIT_FAILURE);
        } else if (WIFSTOPPED(child_exit_status)) {
            fprintf(stderr, "child process was stopped");
            //exit(EXIT_FAILURE);
        }
    }

    if(access(dest, R_OK | W_OK) == -1) {
        fprintf(stderr, "cannot access %s\n", dest);
        exit(EXIT_FAILURE);
    }
}
/*
 * ===  FUNCTION  ======================================================================
 *         Name:  append_files
 *  Description:  Appends files to archive referred to by 'arch'.  If one or files
 *  already in the archive matches a file to be added, calls delete_files()
 *  with the list of the existing files.o
 *
 *  Pre:  'arch' refers to an archive file that is open, readable, and
 *  writable.  'files' contains the pathnames of existing files that are
 *  readable, and 'count' is the number of those files.
 * =====================================================================================
 */
static void
append_files(int arch, char *archname, char **files, int count)
{
    char *new_files[count];
    int new_arch;
    int i;

    if(!(new_arch = isempty_arch(archname))) {
        for(i = 0; i < count; i++)
            new_files[i] = files[i];

        printf("I'm here!");

        /* NULL out any files in 'existing_files'
         * that don't already exist in archive */
        map_over_members(arch, check_members, new_files, count, -1);
    }

    /* Seek to end of file */
    lseek(arch, 0, SEEK_END);

    struct ar_hdr new_hdr;
    char *fname;
    int new_fd;
    size_t to_write;

    for(i = 0; i < count; i++) {
        fname = files[i];
        if(isdir(fname)) {
            fprintf(stderr, "%s is a directory\n", fname);
            exit(EXIT_FAILURE);
        } else if(strcmp(archname, fname) == 0) {
            /* This is something I wish I could
             * have fixed, but my attempts at copying
             * the archive to a temporary file and then
             * appending its contents ate up a lot of
             * time and ended in failure. */
            fprintf(stderr, "cannot add archive to itself\n");
        }
        if(fname != NULL) {
            fill_ar_hdr(fname, &new_hdr);
            if(write(arch, &new_hdr, SARFHDR) == -1) {
                perror("write");
                fprintf(stderr, "failed to write header to archive\n");
                goto cleanup;
            }

            if((new_fd = open(fname, O_RDONLY | O_EXCL)) == -1) {
                perror("open");
                fprintf(stderr, "failed to open file %s\n", fname);
                goto cleanup;
            }

            to_write = get_file_size(new_fd);
            write_to_arch(new_fd, arch, to_write);
        }
    }

    if(!new_arch) {
        for(i = 0; i < count; i++)
            if(new_files[i] != NULL)
                files[i] = NULL;

        for(i = 0; i < count; i++) {
            if(files[i] != NULL) {
                delete_files(arch, archname, files, count);
                break;
            }
        }
    }

cleanup:
    if(close(arch) == -1) {
        perror("read");
        fprintf(stderr, "error closing archive file\n");
        exit(EXIT_FAILURE);
    }

    return;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_member_verbose
 *  Description:  Prints a string representing the name of an archive member.
 *
 *  Name and inspiration due to Dan Albert.
 * =====================================================================================
 */
static void
print_member_concise(int arch, struct ar_hdr *hdr, char **files, int count, int temp_fd)
{
    char fname[SARFNAME+1];
    get_hdr_attr(hdr, name_attr, fname, SARFNAME+1);

    if(count <= 0 || match_fnames(fname, files, count, strcmp) != -1)
        printf("%s\n", fname);
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  print_member_concise
 *  Description:  Prints a string representing the attributes of an archive
 *  member.
 *
 *  Name, inspiration, and string formatting ideas due to Dan Albert.
 * =====================================================================================
 */
static void
print_member_verbose(int arch, struct ar_hdr *hdr, char **files, int count, int temp_fd)
{
    char fname[SARFNAME+1];
    get_hdr_attr(hdr, name_attr, fname, SARFNAME+1);

    if(count > 0 && match_fnames(fname, files, count, strcmp) == -1)
        return;

    struct shadow_hdr shadow;
    fill_shadow_hdr(hdr, &shadow);

    char ftime[18];
    struct tm *time = localtime(&shadow.ar_date);
    strftime(ftime, 18, "%b %e %H:%M %Y", time);

    char modestr[10];
    modestr[9] = '\0';
    strmode(shadow.ar_mode, modestr);

    printf("%-9s %d/%-6d %5lld %s %s\n",
        modestr,
        shadow.ar_uid,
        shadow.ar_gid,
        (long long)shadow.ar_size,
        ftime,
        shadow.ar_name
    );
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  is_reg_file
 *  Description:  Filter function for append_all_files().  Returns 1 if an
 *  entry in the dirent struct is a regular file, 0 otherwise.
 * =====================================================================================
 */
static int
is_reg_file(const struct dirent *entry)
{
    if(entry->d_type == DT_REG) {
        int temp_fd = open(entry->d_name, O_RDONLY);
        if(seek_past_armag(temp_fd) == -1) {
            close(temp_fd);
            return 1;
        } else {
            close(temp_fd);
            return 0;
        }
    }

    return 0;
}

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  append_all_files
 *  Description:  Wrapper for append_files().  Passes a list of all regular
 *  files in the current working directory to append_files().  If 'timeout' >
 *  zero, sleeps for 'timeout' seconds and passes to append_files() only those
 *  files with an mtime that occurred during the sleep() period.
 *
 *  Pre: 'arch' refers to an archive file that exists and is readable and
 *  writable.  Be careful not to pass a negative number as 'timeout', unless
 *  you want to wait a while or hit SIGINT.
 *
 *  Thanks to user R. at
 *  http://stackoverflow.com/questions/4832603/how-could-i-temporary-redirect-stdout-to-a-file-in-a-c-program
 *  for the technique for redirecting stderr
 * =====================================================================================
 */
static void
append_all_files(int arch, char *archname, unsigned int timeout, int (*filter)(const struct dirent *))
{
    time_t start_time = time(NULL);

    if(timeout != 0)
        sleep(timeout);

    time_t end_time = start_time + timeout;

    char *dirname = ".";
    struct dirent **namelist;

    int temp, orig;
    fflush(stderr);
    orig = dup(2);
    temp = open("/dev/null", O_WRONLY);
    dup2(temp, 2);
    close(temp);

    int count = scandir(dirname, &namelist, filter, &alphasort);

    fflush(stderr);
    dup2(orig, 2);
    close(orig);

    char *files[count];
    int i;
    struct stat statbuf;
    for(i = 0; i < count; i++) {
        stat(namelist[i]->d_name, &statbuf);
        if(timeout != 0) {
            if(statbuf.st_mtime > start_time && statbuf.st_mtime < end_time) {
                printf("Adding file %s to archive %s\n", namelist[i]->d_name, archname);
                files[i] = namelist[i]->d_name;
            } else {
                files[i] = NULL;
            }
        } else {
            files[i] = namelist[i]->d_name;
        }
    }

    append_files(arch, archname, files, count);

    for(i = 0; i < count; i++)
        free(namelist[i]);

    free(namelist);
}

int
main (int argc, char **argv)
{
    /* Program requires >= 4 arguments */
    if(argc < 3)
        usage(stderr, argv[0], "Insufficient arguments", 1);

    /* Variable to hold flag currently
     * being ready by getopt() */
    int c;
    int operation_alters_arch = 0;

    unsigned int timeout = 0;

    /* Struct for use with GNU getopt_long() */
    struct option longopts[] = {
        { "quick",      no_argument,        0,  'q'   },
        { "extract",    no_argument,        0,  'x'   },
        { "contents",   no_argument,        0,  't'   },
        { "verbose",    no_argument,        0,  'v'   },
        { "delete",     no_argument,        0,  'd'   },
        { "append",     no_argument,        0,  'A'   },
        { "when",       required_argument,  0,  'w'   },
        { 0, 0, 0, 0 }
    };

    int opt_index = 0;

    /*
     * Loop adapted from example given at
     * http://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     *
     * Loop through command line arguments, incrementing
     * 'seen' when we encounter a mandatory argument.
     *
     * All options besides '-v' require an argument.
     */
    while((c = getopt_long(argc, argv, "qxtvdAw:",
                    longopts, &opt_index)) != -1) {
        switch(c) {
            case 'q':
            case 'x':
            case 't':
            case 'v':
            case 'd':
            case 'A':
            case 'w':
                if(operation != none) {
                    fprintf(stderr, "two different operation options specified\n");
                    exit(EXIT_FAILURE);
                }
                break;
        }

        switch(c) {
            case 'q':
                operation_alters_arch = 1;
                operation = append;
                break;
            case 'x':
                operation = extract;
                break;
            case 't':
                operation = contents;
                break;
            case 'v':
                operation = verbose;
                break;
            case 'd':
                operation_alters_arch = 1;
                operation = delete;
                break;
            case 'A':
                operation_alters_arch = 1;
                operation = append_all;
                break;
            case 'w':
                operation_alters_arch = 1;
                operation = append_all;
                timeout = strtoul(optarg, 0, 10);
                break;
            case('?'):
            default:
                usage(stderr, argv[0], "unrecognized option", 1);
                break;
        }
    }

    /* Print usage and exit if no operation
     * was given */
    if(operation == none)
        usage(stderr, argv[0], "'none' operation specified", 1);

    /* Array for remaining arguments.
     * Assumes remaining arguments
     * are filepaths. */
    int count = argc - optind - 1;

    char *archname = argv[optind++];
    if(archname == NULL)
        usage(stderr, argv[0], "no archive file specified", 1);
    int arch;

    /* Temporarily set umask to 0000 in order to
     * create file with permissions 0666 using
     * DFLTMODE */
    umask(0000);
    if((operation == append
            || operation == append_all)
            && access(archname, R_OK | W_OK) == -1) {
        printf("%s: creating %s\n", argv[0], archname);
        arch = open(archname, O_CREAT | O_RDWR, DFLTMODE);
    } else {
        arch = open(archname, O_RDWR, DFLTMODE);
    }
    umask(DFLTUMASK);

    if(arch == -1) {
        fprintf(stderr, "error opening archive\n");
        perror("open");
        exit(EXIT_FAILURE);
    }

    if(isempty(archname))
        write_armag(arch);

    char *files[count];
    int i;

    for(i = 0; optind < argc; i++, optind++)
        files[i] = argv[optind];

    switch(operation) {
        case append:
            append_files(arch, archname, files, count);
            break;
        case append_all:
            if(count != 0 && timeout == 0)
                usage(stderr, argv[0], "-A option takes no arguments", 1);
            else if(count != 0)
                usage(stderr, argv[0], "-w option takes no arguments", 1);
            append_all_files(arch, archname, timeout, &is_reg_file);
            break;
        case extract:
            extract_files(arch, files, count);
            break;
        case contents:
            map_over_members(arch, &print_member_concise, files, count, -1);
            break;
        case verbose:
            map_over_members(arch, &print_member_verbose, files, count, -1);
            break;
        case delete:
            delete_files(arch, archname, files, count);
            break;
        case none:
        default:
            usage(stderr, argv[0], "no operation option specified", 1);
    }

    if(operation_alters_arch != 1) {
        if(close(arch) == -1) {
            fprintf(stderr, "failed to close archive %s\n", archname);
            perror("close");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
