# ft_ssl

OpenSSL from scratch as a 42 school project ([subject](en.subject.pdf)).\
NOTE Implemented using a polymorphic architecture in order to optimise scaling and pattern reutilization.

### I) Suported algorithms:
- [md5](https://en.wikipedia.org/wiki/MD5)
- [sha0](https://en.wikipedia.org/wiki/SHA-1)
- [sha1](https://en.wikipedia.org/wiki/SHA-1)
- [sha224](https://en.wikipedia.org/wiki/SHA-2)
- [sha256](https://en.wikipedia.org/wiki/SHA-2)
- [sha384](https://en.wikipedia.org/wiki/SHA-2)
- [sha512](https://en.wikipedia.org/wiki/SHA-2)

### II) Build
```sh
./gensrcs.sh && make
```
### III) Usage
```sh
./ft_ssl <algorithm> [-hpqr] [-s <string>] [files*] # Also reads input from sdtin
```
- `-h`: Display help.
- `-p`: Output stdin.
- `-q`: Quiet mode.
- `-r`: Reverse output.
- `-s`: Input string from CLI.

> NOTE: If `<algorithm>` is not provided the options are parsed from stdin as OpenSSL does.

### IV) Quality check
```
./tester.sh
```
