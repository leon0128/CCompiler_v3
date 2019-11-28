#include "generator.hpp"
#include "debugger.hpp"
#include "token.hpp"

Generator::Generator():
    mParent(nullptr),
    mAssembly(),
    mIsValid(true)
{
}

Generator::~Generator()
{
}

bool Generator::operator()(Token* parent,
                           std::stringstream& assembly)
{
    mParent = parent;

    generate();

    Debugger::generator(mAssembly);

    assembly.swap(mAssembly);
    return mIsValid;
}

void Generator::generate()
{
    mAssembly << ".intel_syntax noprefix\n"
              << ".global main\n\n"
              << "main:\n";

    

}