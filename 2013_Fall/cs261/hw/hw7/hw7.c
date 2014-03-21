/* Method 1: delete link */
void insertMap (struct hashMap * ht, KeyType k, ValueType v) {
    ht->count++;
    if(tableLoad(ht) >= (float)LOAD_FACTOR_THRESHOLD)
        _setTableSize(ht, ht->tableSize*2);
    int i = _getHash(ht, k, hashFunc);
    struct hashLink *curr = ht->table[i];
    struct hashLink *prev = NULL;
    while(curr != NULL && _compare(curr->key, k) != 0) {
        if(_compare(curr->key, k) == 0) {
            _freeLink(curr, ht);
            break;
        }
        prev = curr;
        curr = _nextLink(curr);
    }
    curr = malloc(sizeof(struct hashLink));
    assert(curr != NULL);
    size_t keyMemBuf = (strlen(k) + 1) * sizeof(char);
    curr->key = malloc(keyMemBuf);
    memcpy(curr->key, k, keyMemBuf);
    curr->value = v;
    curr->next = NULL;
    if(prev != NULL)
        prev->next = curr;
    else
        ht->table[i] = curr;
}

/* Method 2: Replace value, but don't delete link */
void insertMap (struct hashMap * ht, KeyType k, ValueType v) {
    ht->count++;
    if(tableLoad(ht) >= (float)LOAD_FACTOR_THRESHOLD)
        _setTableSize(ht, ht->tableSize*2);
    int i = _getHash(ht, k, hashFunc);
    struct hashLink *curr = ht->table[i];
    struct hashLink *prev = NULL;
    while(curr != NULL) {
        if(_compare(curr->key, k) == 0) {
            curr->value = v;                /* These two lines differ from Method 1 */
            return;
        }
        prev = curr;
        curr = _nextLink(curr);
    }
    curr = malloc(sizeof(struct hashLink));
    assert(curr != NULL);
    size_t keyMemBuf = (strlen(k) + 1) * sizeof(char);
    curr->key = malloc(keyMemBuf);
    memcpy(curr->key, k, keyMemBuf);
    curr->value = v;
    curr->next = NULL;
    if(prev != NULL)
        prev->next = curr;
    else
        ht->table[i] = curr;
}

