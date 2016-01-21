# gstruct

General struct

## 用法

### char

```
gstruct *gs = gstruct_new();
gstruct_add_char(gs, 'a');
gstruct_apply_data(gs);
printf("%c\n", GS_CHR_P(gs));
gstruct_free(gs);
```
### int

```
gstruct *gs = gstruct_new();
gstruct_add_int(gs, 123);
gstruct_apply_data(gs);
printf("%d\n", GS_INT_P(gs));
gstruct_free(gs);
```

### long

```
gstruct *gs = gstruct_new();
gstruct_add_long(gs, 121);
gstruct_apply_data(gs);
printf("%d\n", GS_LONG_P(gs));
```
### double & float

```
gstruct *gs = gstruct_new();
gstruct_add_double(gs, 121.11);
gstruct_apply_data(gs);
printf("%f\n", GS_DOUBLE_P(gs));
gstruct_free(gs);
```

### string

```
gstruct *gs = gstruct_new();
char str[] = "hello world";
gstruct_add_str(gs, strlen(str));
gstruct_add_str_body(gs, str, strlen(str));
gstruct_apply_data(gs);
printf("string length:%d, string:%s\n",
    GS_STR_SIZE_P(gs),
    GS_STR_PTR_P(gs)
);
gstruct_free(gs);
```

### array
```
gstruct *gs = gstruct_new();
gstruct_add_array(gs, 3);

char *str1 = "str1";
gstruct_add_str(gs, strlen(str1));
gstruct_add_str_body(gs, str1, strlen(str1));

char *str2 = "str2";
gstruct_add_str(gs, strlen(str2));
gstruct_add_str_body(gs, str2, strlen(str2));

char *str3 = "str3";
gstruct_add_str(gs, strlen(str3));
gstruct_add_str_body(gs, str3, strlen(str3));

gstruct_apply_data(gs);

printf("array size:%d, \nstr1: size:%d, s:%s, \nstr2: size:%d, s:%s, \nstr3: size:%d, s:%s\n",
    GS_ARRSIZE_P(gs),
    gs->via.array.ptr[0].via.str.size,
    GS_STR_PTR(GS_ARRVAL_P(gs, 0)),
    gs->via.array.ptr[1].via.str.size,
    GS_STR_PTR(GS_ARRVAL_P(gs, 1)),
    gs->via.array.ptr[2].via.str.size,
    GS_STR_PTR(GS_ARRVAL_P(gs, 2))
);
gstruct_free(gs);
```

### map

```
gstruct *gs = gstruct_new();
gstruct_add_map(gs, 2);

char *key1 = "key1";
gstruct_add_str(gs, strlen(key1));
gstruct_add_str_body(gs, key1, strlen(key1));

char *val1 = "val1";
gstruct_add_str(gs, strlen(val1));
gstruct_add_str_body(gs, val1, strlen(val1));

char *key2 = "key2";
gstruct_add_str(gs, strlen(key2));
gstruct_add_str_body(gs, key2, strlen(key2));

char *val2 = "val2";
gstruct_add_str(gs, strlen(val2));
gstruct_add_str_body(gs, val2, strlen(val2));

gstruct_apply_data(gs);

gstruct_free(gs);
```

### bin

```
gstruct *gs = gstruct_new();
char bin[] = "abcdefg12345";
gstruct_add_bin(gs, sizeof(bin));
gstruct_add_bin_body(gs, bin, sizeof(bin));
gstruct_apply_data(gs);
```
