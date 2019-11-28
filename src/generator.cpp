#include "generator.hpp"
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
                           std::string& assembly)
{
    mParent = parent;

    generate();

    assembly.swap(mAssembly);
    return mIsValid;
}