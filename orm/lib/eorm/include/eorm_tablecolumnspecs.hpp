#pragma once

namespace eorm {

enum TableColumnSpecs
{
    EMPTY,
    PRIMARY_KEY,
    AUTOINCREMENT,
    DEFAULT=4,
    NOT_NULL=8
};

typedef TableColumnSpecs TCS;

static bool is_PRIMARY_KEY     (TableColumnSpecs specs) { return ( specs & PRIMARY_KEY   ); }
static bool is_AUTOINCREMENT   (TableColumnSpecs specs) { return ( specs & AUTOINCREMENT ); }
static bool is_NOT_NULL        (TableColumnSpecs specs) { return ( specs & NOT_NULL      ); }
static bool is_DEFAULT         (TableColumnSpecs specs) { return ( specs & DEFAULT       ); }

}
