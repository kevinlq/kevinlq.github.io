---
layout : life
title: 最为常用的HASH算法
category : c++学习
duoshuo: true
date : 2017-03-08
---

        作者: daodaoliang
        版本: V 0.0.1
        日期: 2017年03月08日

 murmurhash2算法 和 DJB Hash算法是目前最流行的hash算法

<!-- more -->

### 1.DJB HASH算法

~~~c++
/* the famous DJB Hash Function for strings */
unsigned int DJBHash(char *str)
{
    unsigned int hash = 5381;

    while (*str){
       hash = ((hash << 5) + hash) + (*str++); /* times 33 */
    }
    hash &= ~(1 << 31); /* strip the highest bit */
    return hash;
 }
~~~

### 2.murmurhash2

```c++
uint32_t
murmur_hash2(char *data, size_t len)
{  
    uint32_t  h, k;
  
    h = 0 ^ len;
  
    while (len >= 4) {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;
  
        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
    case 3:
        h ^= data[2] << 16;
    case 2:
        h ^= data[1] << 8;
    case 1:
        h ^= data[0];
        h *= 0x5bd1e995;
    }

    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;

    return h;
}
```

