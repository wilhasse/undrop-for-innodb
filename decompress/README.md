# Linux

Worked in Debian 10 ISO  
https://img.cs.montana.edu/linux/debian/10/amd64/


# Install

```bash
sudo apt-get install \
    build-essential \
    git \
    libncurses5-dev \
    libncurses-dev \
    libpcre2-8-0 \
    libarchive13 \
    zlib1g-dev \
    wget \
    libaio-dev \
    locate \
    cmake \
    librhash0 \
    m4 \
    libpsl5 \
    libfl-dev \
    librtmp1 \
    pigz
```

# Download

Git Relase:  
https://github.com/mysql/mysql-server/releases/tag/mysql-5.5.16

Download MySQL 5.5.16  
https://github.com/mysql/mysql-server/archive/refs/tags/mysql-5.5.16.tar.gz

Install Bison 
Note: It doesn't compile with bison package in Debian 10

```bash
# remove if present
apt-get remove bison

# source
wget http://ftp.gnu.org/gnu/bison/bison-2.3.tar.gz
tar xzf bison-2.3.tar.gz
cd bison-2.3
./configure
make
sudo make install
```

# Compile

```bash
# download and unpack
git clone https://github.com/wilhasse/undrop-for-innodb
cd decompress
wget https://github.com/mysql/mysql-server/archive/refs/tags/mysql-5.5.16.tar.gz
tar xvzf mysql-server-mysql-5.5.16
cd mysql-server-mysql-5.5.16

# compile mysql
cmake -DCMAKE_C_FLAGS="-Wno-error -Wno-implicit-fallthrough -Wno-sizeof-pointer-memaccess -Wno-deprecated-declarations -Wno-unused-local-typedefs -Wno-unused-value -Wno-deprecated -Wno-cast-function-type -Wno-nonnull-compare -Wno-shift-negative-value -Wno-logical-not-parentheses -Wno-misleading-indentation -Wno-format-overflow -Wno-format-truncation" -DCMAKE_CXX_FLAGS="-fpermissive -Wno-deprecated -Wno-cast-function-type -Wno-implicit-fallthrough -Wno-nonnull-compare -Wno-narrowing -Wno-shift-negative-value -Wno-int-in-bool-context -Wno-unused-local-typedefs -Wno-sizeof-pointer-memaccess -Wno-class-memaccess -Wno-parentheses -Wno-unused-function"
make

# if successfull go back to decompress and make it
cd ..
make
```

# Recovery

```bash
# extract pages
./stream_parser -f TABLE.ibd -V

# dir
cslog@mysql55:~/undrop-for-innodb$ ls -la pages-TABLE.ibd/

#drwxr-xr-x  4 test test 4096 set 27 13:04 .
#drwxr-xr-x 11 test test 4096 set 27 13:14 ..
#drwxr-xr-x  2 test test 4096 set 27 13:04 FIL_PAGE_INDEX
#drwxr-xr-x  2 test test 4096 set 27 13:04 FIL_PAGE_TYPE_BLOB

# genarate sql format in output.txt , only that has been deleted
 ./c_parser -5 -d -D -s -f pages-TABLE.ibd/FIL_PAGE_INDEX/ -t table_definition.sql
```
