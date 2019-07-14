#pragma once
#include <vector>

struct Note;

class FurElise
{
public:
    FurElise();
    ~FurElise() = default;

    static void Generate(_Out_ std::vector<Note>& notes);
};