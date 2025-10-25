#include <functional>

#include <QString>
#include <QTextCharFormat>
#include <QRegularExpression>


#include "ElementExtract.h"

namespace PostProcessPass
{
    using reset_block_fn_t = std::function<void(uint32_t id)>;
    using block_destroyed_fn_t = std::function<void(uint32_t id)>;
    using format_phrase_fn_t = std::function<QTextCharFormat(uint32_t blockId, int start, int end, const QString& phrase)>;

    using T = std::tuple<QRegularExpression, reset_block_fn_t, block_destroyed_fn_t, format_phrase_fn_t>;

    ELEMENT_EXTRACT(T, 0, phrase_regex)
    ELEMENT_EXTRACT(T, 1, reset_block)
    ELEMENT_EXTRACT(T, 2, block_destroyed)
    ELEMENT_EXTRACT(T, 3, format_phrase)
}
using post_process_pass_t = PostProcessPass::T;

