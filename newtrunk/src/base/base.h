
#ifndef __BASE_H
#define __BASE_H

#include "typer.h"
#include "common.h"
#include "../tools/convert.h"

// inserted by NJH Feb 2003
class base {
    protected:
        string       Name;
        int          Index;
        const base * Owner;
        convert      conv;

    public:
        base();

        base(string       aName,
             const int    aIndex = -1,
             const base * aOwner = nullptr);

        base(const base & aBase);

        virtual ~base();

        // Assignment operator
        base & operator = (const base & aBase);

        void InitVariables();

        void SetOwner(const base * aOwner) {
            Owner = aOwner;
        }

        string GetName(void) const {
            return Name;
        }

        const base * GetOwner() {
            return Owner;
        }

        string GetLongName();

        string GetLongNameInNumbers();

        int GetIndex() {
            return Index;
        }

        void SetIndex(int anIndex) {
            Index = anIndex;
        }
};
#endif

