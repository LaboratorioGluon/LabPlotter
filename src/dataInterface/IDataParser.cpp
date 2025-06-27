// dataInterface/IDataParser.cpp
#include "IDataParser.h"

// Define el constructor de IDataParser
// Esto es necesario para que el linker encuentre la definición de QObject::QObject
// y para inicializar la vtable (tabla de funciones virtuales) de IDataParser.
IDataParser::IDataParser(QObject *parent) : QObject(parent)
{
    // No hay necesidad de lógica aquí, solo la definición básica.
}

// Define el destructor de IDataParser
// Aunque es virtual y default, su definición debe estar en un .cpp
IDataParser::~IDataParser()
{
    // No hay necesidad de lógica aquí.
}