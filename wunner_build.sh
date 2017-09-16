#! /bin/sh

compiler="g++"
if [ $# -eq 1 ]
  then
    compiler=$1
fi
compiler="$compiler -std=c++14 -Wall"

mkdir -p build/bin

echo "Generating object files for wunner libraries..."
$compiler -c -Iinc src/autocomplete.cpp -o build/autocomplete.o
$compiler -c -Iinc src/crawler.cpp -o build/crawler.o
$compiler -c -Iinc src/index_builder.cpp -o build/index_builder.o
$compiler -c -Iinc -Ilib src/parser.cpp -o build/parser.o
$compiler -c -Iinc src/query.cpp -o build/query.o
$compiler -c -Iinc src/validator.cpp -o build/validator.o

echo "Generating object files for library Porter Stemmer..."
$compiler -c lib/PorterStemmer/porter2_stemmer.cpp -o build/porter_stemmer.o

cd build

echo "Creating static library wunner_libs..."
ar rcs libwunner_libs.a autocomplete.o crawler.o index_builder.o parser.o query.o validator.o

echo "Creating static library Porter Stemmer..."
ar rcs libporter_stemmer.a porter_stemmer.o

rm *.o

cd ..
echo "Creating binary wunner_search..."
$compiler -static -Iinc src/main.cpp -Lbuild -lwunner_libs -lporter_stemmer -lpthread -o build/bin/wunner_search
file build/bin/wunner_search
