- change `sprintf` to `os_sprintf`
- add:

```
#elif CMAKE_SYSTEM_PROCESSOR == ESP8266
static void *internal_malloc(size_t size)
{
    return os_malloc(size);
}
static void internal_free(void *pointer)
{
    os_free(pointer);
}
static void *internal_realloc(void *pointer, size_t size)
{
    return os_realloc(pointer, size);
}
```
