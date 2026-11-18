// SPDX-License-Identifier: GPL-2.0-only
#include "../src/Bot/Cmd/ChatClassText.h"

#include <cassert>

int main()
{
    assert(ChatClassText::FormatClass("holy", "priest") == "holy priest");
    return 0;
}
