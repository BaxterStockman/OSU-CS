#!/usr/bin/env bash

#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 6

function create_backup()
{
    file=$1
    counter=1
    while [[ -e "$file.$counter.bak" ]]; do
        counter=$[counter+1]
    done
    cp $file "$file.$counter.bak"
}

function diff_twins()
{
    # Program to invoke
    prog=$1
    # File containing twin primes to use as basis for comparison
    comparf=$2
    # Limit to pass to prime finder
    MAX=$3

    $prog -c 5 -m $MAX > "$prog.out"

    # If the output of diff is empty, then the
    # two files are identical
    if [[ -z `diff $prog.out $comparf` ]]; then
        echo "$prog found twins properly"
    else
        echo "$prog did not find twins properly"
    fi
}

function create_csv()
{
    # Program to invoke
    prog=$1
    # Output file
    outf=$2
    # Initial upper limit to pass to prime finder
    INIT=$3
    # Current limit to pass to prime finder
    NOW=$3
    # Highest limit to pass to prime finder
    LIM=$4
    # Loop control variable -- set to zero when LIM reached.
    LOOP=1

    # if 'outf' exists, create a backup
    [[ -e $outf ]] && create_backup $outf

    echo "Timing $prog..."
    echo "limit,concurrency,minutes,seconds" | tee "$outf"
    for concur in {0..10..2}; do
        [[ $concur -eq 0 ]] && concur=$[concur+1]
        while (( $LOOP )); do
            if (( $NOW >= $LIM )); then
                NOW=$LIM
                LOOP=0
            fi
            exec_time=$( time ($prog -q -c $concur -m $NOW) 2>&1 1>/dev/null )
            cs_time=`echo $exec_time | perl -ne '/\Areal\s+(\d+)m(\d+\.\d+)s.*\Z/ and print "$1,$2\n"'`
            echo "$NOW,$concur,$cs_time" | tee -a "$outf"
            NOW=$[NOW+INIT]
        done
        NOW=$INIT
        LOOP=1
    done
}

threadf="primePThread"
procf="primeMProc"
outthreadf="$threadf.csv"
outprocf="$procf.csv"
primesf="primes1.txt"
compf="twins.txt"
compc=5
targets="$threadf $procf"

UINT_MAX=`./getdefs.pl limits.h UINT_MAX`
MAX_VAL=$UINT_MAX
MAX_INIT=500000000

# Largest twin prime in the
# first million primes
MAX_TWIN=15485653

while (( $# )); do
    case $1 in
        "-t")
            threadf=$2
            ;;
        "-p")
            procf=$2
            ;;
        "-k")
            checkf=$2
            ;;
        "-s")
            MAX_INIT=$2
            if [[ $MAX_INIT > $UINT_MAX ]]; then
                echo "$0: -s 0-$UINT_MAX"
                exit 1
            fi
            ;;
        "-m")
            MAX_VAL=$2
            if [[ $MAX_VAL > $UINT_MAX ]]; then
                echo "$0: -m 0-$UINT_MAX"
                exit 1
            fi
            ;;
        "-T")
            outthreadf=$2
            ;;
        "-P")
            outprocf=$2
            ;;
        "-O")
            outtimef=$2
            ;;
        *)
            echo "$0: Unknown option; exiting"
            exit 1
            ;;
    esac
    shift
    shift
done

make $targets
if [[ ! -e $threadf || ! -e $procf ]]; then
    echo "$0: Failed to create executables; exiting"
    exit 0
fi

# Call gettwins.pl to parse list
# of primes for twin primes; output
# to 'compf'
./gettwins.pl $primesf > $compf

diff_twins "./$threadf" $compf $MAX_TWIN
diff_twins "./$procf" $compf $MAX_TWIN

create_csv "./$threadf" $outthreadf $MAX_INIT $MAX_VAL
create_csv "./$procf" $outprocf $MAX_INIT $MAX_VAL
