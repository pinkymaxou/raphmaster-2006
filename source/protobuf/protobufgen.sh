
python3 ../../tools/nanopb/generator/nanopb_generator.py cocktaildb.proto
mkdir -p csharp
protoc --csharp_out=csharp cocktaildb.proto