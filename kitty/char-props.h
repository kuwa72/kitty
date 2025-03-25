/*
 * char-props.h
 * Copyright (C) 2025 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#pragma once

#include "data-types.h"

// CharPropsDeclaration
typedef union CharProps {
    struct {
        uint8_t shifted_width : 3;
        uint8_t is_extended_pictographic : 1;
        uint8_t grapheme_break : 4;
        uint8_t indic_conjunct_break : 2;
        uint8_t is_emoji : 1;
        uint8_t is_emoji_presentation_base : 1;
        uint8_t is_invalid : 1;
        uint8_t is_non_rendered : 1;
        uint8_t is_symbol : 1;
        uint8_t is_combining_char : 1;
    };
    uint16_t val;
} CharProps;
static_assert(sizeof(CharProps) == sizeof(uint16_t), "Fix the ordering of CharProps");
// EndCharPropsDeclaration

typedef struct GraphemeSegmentationState {
    int last_char_prop;

    /* True if the last character ends a sequence of Indic_Conjunct_Break
values:  consonant {extend|linker}*  */
    bool incb_consonant_extended;
    /* True if the last character ends a sequence of Indic_Conjunct_Break
values:  consonant {extend|linker}* linker  */
    bool incb_consonant_extended_linker;
    /* True if the last character ends a sequence of Indic_Conjunct_Break
values:  consonant {extend|linker}* linker {extend|linker}*  */
    bool incb_consonant_extended_linker_extended;

    /* True if the last character ends an emoji modifier sequence
       \p{Extended_Pictographic} Extend*.  */
    bool emoji_modifier_sequence;
    /* True if the last character was immediately preceded by an
       emoji modifier sequence   \p{Extended_Pictographic} Extend*.  */
    bool emoji_modifier_sequence_before_last_char;

    /* Number of consecutive regional indicator (RI) characters seen
       immediately before the current point.  */
    size_t ri_count;
} GraphemeSegmentationState;

CharProps char_props_for(char_type ch);
void grapheme_segmentation_reset(GraphemeSegmentationState *s);
bool grapheme_segmentation_step(GraphemeSegmentationState *s, CharProps ch);
static inline int wcwidth_std(CharProps ch) { return (int)ch.shifted_width - 4/*=width_shift*/; }
