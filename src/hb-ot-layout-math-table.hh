/*
 * Copyright © 2016  Igalia S.L.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Igalia Author(s): Frédéric Wang
 */

#ifndef HB_OT_LAYOUT_MATH_TABLE_HH
#define HB_OT_LAYOUT_MATH_TABLE_HH

#include "hb-open-type-private.hh"
#include "hb-ot-layout-common-private.hh"
#include "hb-ot-math.h"

namespace OT {


struct MathValueRecord
{
  inline hb_position_t get_x_value (hb_font_t *font, const void *base) const
  { return font->em_scale_x (value) + (base+deviceTable).get_x_delta (font); }
  inline hb_position_t get_y_value (hb_font_t *font, const void *base) const
  { return font->em_scale_y (value) + (base+deviceTable).get_y_delta (font); }

  inline bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && deviceTable.sanitize (c, base));
  }

protected:
  SHORT			value;		/* The X or Y value in design units */
  OffsetTo<Device>	deviceTable;	/* Offset to the device table - from the
					 * beginning of parent table. May be NULL.
					 * Suggested format for device table is 1. */

public:
  DEFINE_SIZE_STATIC (4);
};

struct MathConstants
{
  inline bool sanitize_math_value_records (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);

    unsigned int count = ARRAY_LENGTH (mathValueRecords);
    for (unsigned int i = 0; i < count; i++)
      if (!mathValueRecords[i].sanitize (c, this))
	return_trace (false);

    return_trace (true);
  }

  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && sanitize_math_value_records(c));
  }

  inline hb_position_t get_value (hb_font_t *font, hb_ot_math_constant_t constant) const
  {
    switch (constant) {

    case HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN:
    case HB_OT_MATH_CONSTANT_SCRIPT_SCRIPT_PERCENT_SCALE_DOWN:
      return percentScaleDown[constant - HB_OT_MATH_CONSTANT_SCRIPT_PERCENT_SCALE_DOWN];

    case HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT:
    case HB_OT_MATH_CONSTANT_DISPLAY_OPERATOR_MIN_HEIGHT:
      return font->em_scale_y (minHeight[constant - HB_OT_MATH_CONSTANT_DELIMITED_SUB_FORMULA_MIN_HEIGHT]);

    case HB_OT_MATH_CONSTANT_RADICAL_KERN_AFTER_DEGREE:
    case HB_OT_MATH_CONSTANT_RADICAL_KERN_BEFORE_DEGREE:
    case HB_OT_MATH_CONSTANT_SKEWED_FRACTION_HORIZONTAL_GAP:
    case HB_OT_MATH_CONSTANT_SPACE_AFTER_SCRIPT:
      return mathValueRecords[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_x_value(font, this);

    case HB_OT_MATH_CONSTANT_ACCENT_BASE_HEIGHT:
    case HB_OT_MATH_CONSTANT_AXIS_HEIGHT:
    case HB_OT_MATH_CONSTANT_FLATTENED_ACCENT_BASE_HEIGHT:
    case HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_DISPLAY_STYLE_SHIFT_DOWN:
    case HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_GAP_MIN:
    case HB_OT_MATH_CONSTANT_FRACTION_DENOMINATOR_SHIFT_DOWN:
    case HB_OT_MATH_CONSTANT_FRACTION_DENOM_DISPLAY_STYLE_GAP_MIN:
    case HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_DISPLAY_STYLE_SHIFT_UP:
    case HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_GAP_MIN:
    case HB_OT_MATH_CONSTANT_FRACTION_NUMERATOR_SHIFT_UP:
    case HB_OT_MATH_CONSTANT_FRACTION_NUM_DISPLAY_STYLE_GAP_MIN:
    case HB_OT_MATH_CONSTANT_FRACTION_RULE_THICKNESS:
    case HB_OT_MATH_CONSTANT_LOWER_LIMIT_BASELINE_DROP_MIN:
    case HB_OT_MATH_CONSTANT_LOWER_LIMIT_GAP_MIN:
    case HB_OT_MATH_CONSTANT_MATH_LEADING:
    case HB_OT_MATH_CONSTANT_OVERBAR_EXTRA_ASCENDER:
    case HB_OT_MATH_CONSTANT_OVERBAR_RULE_THICKNESS:
    case HB_OT_MATH_CONSTANT_OVERBAR_VERTICAL_GAP:
    case HB_OT_MATH_CONSTANT_RADICAL_DISPLAY_STYLE_VERTICAL_GAP:
    case HB_OT_MATH_CONSTANT_RADICAL_EXTRA_ASCENDER:
    case HB_OT_MATH_CONSTANT_RADICAL_RULE_THICKNESS:
    case HB_OT_MATH_CONSTANT_RADICAL_VERTICAL_GAP:
    case HB_OT_MATH_CONSTANT_SKEWED_FRACTION_VERTICAL_GAP:
    case HB_OT_MATH_CONSTANT_STACK_BOTTOM_DISPLAY_STYLE_SHIFT_DOWN:
    case HB_OT_MATH_CONSTANT_STACK_BOTTOM_SHIFT_DOWN:
    case HB_OT_MATH_CONSTANT_STACK_DISPLAY_STYLE_GAP_MIN:
    case HB_OT_MATH_CONSTANT_STACK_GAP_MIN:
    case HB_OT_MATH_CONSTANT_STACK_TOP_DISPLAY_STYLE_SHIFT_UP:
    case HB_OT_MATH_CONSTANT_STACK_TOP_SHIFT_UP:
    case HB_OT_MATH_CONSTANT_STRETCH_STACK_BOTTOM_SHIFT_DOWN:
    case HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_ABOVE_MIN:
    case HB_OT_MATH_CONSTANT_STRETCH_STACK_GAP_BELOW_MIN:
    case HB_OT_MATH_CONSTANT_STRETCH_STACK_TOP_SHIFT_UP:
    case HB_OT_MATH_CONSTANT_SUBSCRIPT_BASELINE_DROP_MIN:
    case HB_OT_MATH_CONSTANT_SUBSCRIPT_SHIFT_DOWN:
    case HB_OT_MATH_CONSTANT_SUBSCRIPT_TOP_MAX:
    case HB_OT_MATH_CONSTANT_SUB_SUPERSCRIPT_GAP_MIN:
    case HB_OT_MATH_CONSTANT_SUPERSCRIPT_BASELINE_DROP_MAX:
    case HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MAX_WITH_SUBSCRIPT:
    case HB_OT_MATH_CONSTANT_SUPERSCRIPT_BOTTOM_MIN:
    case HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP:
    case HB_OT_MATH_CONSTANT_SUPERSCRIPT_SHIFT_UP_CRAMPED:
    case HB_OT_MATH_CONSTANT_UNDERBAR_EXTRA_DESCENDER:
    case HB_OT_MATH_CONSTANT_UNDERBAR_RULE_THICKNESS:
    case HB_OT_MATH_CONSTANT_UNDERBAR_VERTICAL_GAP:
    case HB_OT_MATH_CONSTANT_UPPER_LIMIT_BASELINE_RISE_MIN:
    case HB_OT_MATH_CONSTANT_UPPER_LIMIT_GAP_MIN:
      return mathValueRecords[constant - HB_OT_MATH_CONSTANT_MATH_LEADING].get_y_value(font, this);

    case HB_OT_MATH_CONSTANT_RADICAL_DEGREE_BOTTOM_RAISE_PERCENT:
      return radicalDegreeBottomRaisePercent;

    default:
      return 0;
    }
  }

protected:
  SHORT percentScaleDown[2];
  USHORT minHeight[2];
  MathValueRecord mathValueRecords[51];
  SHORT radicalDegreeBottomRaisePercent;

public:
  DEFINE_SIZE_STATIC (214);
};

struct MathItalicsCorrectionInfo
{
  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  coverage.sanitize (c, this) &&
		  italicsCorrection.sanitize (c, this));
  }

  inline bool get_value (hb_font_t *font, hb_codepoint_t glyph,
			 hb_position_t &value) const
  {
    unsigned int index = (this+coverage).get_coverage (glyph);
    if (likely (index == NOT_COVERED)) return false;
    if (unlikely (index >= italicsCorrection.len)) return false;
    value = italicsCorrection[index].get_x_value(font, this);
    return true;
  }

protected:
  OffsetTo<Coverage>       coverage;		/* Offset to Coverage table -
						 * from the beginning of
						 * MathItalicsCorrectionInfo
						 * table. */
  ArrayOf<MathValueRecord> italicsCorrection;	/* Array of MathValueRecords
						 * defining italics correction
						 * values for each
						 * covered glyph. */

public:
  DEFINE_SIZE_ARRAY (4, italicsCorrection);
};

struct MathTopAccentAttachment
{
  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  topAccentCoverage.sanitize (c, this) &&
		  topAccentAttachment.sanitize (c, this));
  }

  inline bool get_value (hb_font_t *font, hb_codepoint_t glyph,
			 hb_position_t &value) const
  {
    unsigned int index = (this+topAccentCoverage).get_coverage (glyph);
    if (likely (index == NOT_COVERED)) return false;
    if (unlikely (index >= topAccentAttachment.len)) return false;
    value = topAccentAttachment[index].get_x_value(font, this);
    return true;
  }

protected:
  OffsetTo<Coverage>       topAccentCoverage;   /* Offset to Coverage table -
						 * from the beginning of
						 * MathTopAccentAttachment
						 * table. */
  ArrayOf<MathValueRecord> topAccentAttachment; /* Array of MathValueRecords
						 * defining top accent
						 * attachment points for each
						 * covered glyph. */

public:
  DEFINE_SIZE_ARRAY (2 + 2, topAccentAttachment);
};

struct MathKern
{
  inline bool sanitize_math_value_records (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    unsigned int count = 2 * heightCount + 1;
    for (unsigned int i = 0; i < count; i++)
      if (!mathValueRecords[i].sanitize (c, this)) return_trace (false);
    return_trace (true);
  }

  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  c->check_array (mathValueRecords,
				  mathValueRecords[0].static_size,
				  2 * heightCount + 1) &&
		  sanitize_math_value_records (c));
  }

  inline hb_position_t get_value (hb_font_t *font,
				  hb_position_t &correction_height) const
  {
    const MathValueRecord* correctionHeight = mathValueRecords;
    const MathValueRecord* kernValue = mathValueRecords + heightCount;
    // The description of the MathKern table is a ambiguous, but interpreting
    // "between the two heights found at those indexes" for 0 < i < len as
    //
    //   correctionHeight[i-1] < correction_height <= correctionHeight[i]
    //
    // makes the result consistent with the limit cases and we can just use the
    // binary search algorithm of std::upper_bound:
    unsigned int count = heightCount;
    unsigned int i = 0;
    while (count > 0) {
      unsigned int half = count / 2;
      hb_position_t height =
	correctionHeight[i + half].get_y_value(font, this);
      if (height < correction_height) {
	i += half + 1;
	count -= half + 1;
      } else
	count = half;
    }
    return kernValue[i].get_x_value(font, this);
  }

protected:
  USHORT          heightCount;
  MathValueRecord mathValueRecords[VAR]; /* Array of correction heights at
					  * which the kern value changes.
					  * Sorted by the height value in
					  * design units. */
					 /* Array of kern values corresponding
					  * to heights. */

public:
  DEFINE_SIZE_ARRAY (2, mathValueRecords);
};

struct MathKernInfoRecord
{
  inline bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);

    unsigned int count = ARRAY_LENGTH (mathKern);
    for (unsigned int i = 0; i < count; i++)
      if (unlikely (!mathKern[i].sanitize (c, base)))
	return_trace (false);

    return_trace (true);
  }

  inline const MathKern &get_math_kern (hb_ot_math_kern_t kern,
					const void *base) const
  {
    unsigned int idx = kern;
    if (unlikely (idx > ARRAY_LENGTH (mathKern))) return Null(MathKern);
    return base+mathKern[idx];
  }

protected:
  /* Offset to MathKern table for each corner -
   * from the beginning of MathKernInfo table. May be NULL. */
  OffsetTo<MathKern> mathKern[4];

public:
  DEFINE_SIZE_STATIC (8);
};

struct MathKernInfo
{
  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  mathKernCoverage.sanitize (c, this) &&
		  mathKernInfoRecords.sanitize (c, this));
  }

  inline const MathKernInfoRecord&
  get_math_kern_info_record (hb_codepoint_t glyph) const
  {
    unsigned int index = (this+mathKernCoverage).get_coverage (glyph);
    return mathKernInfoRecords[index];
  }

protected:
  OffsetTo<Coverage>		mathKernCoverage;    /* Offset to Coverage table -
						      * from the beginning of the
						      * MathKernInfo table. */
  ArrayOf<MathKernInfoRecord>	mathKernInfoRecords; /* Array of
						      * MathKernInfoRecords,
						      * per-glyph information for
						      * mathematical positioning
						      * of subscripts and
						      * superscripts. */

public:
  DEFINE_SIZE_ARRAY (4, mathKernInfoRecords);
};

struct MathGlyphInfo
{
  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  mathItalicsCorrectionInfo.sanitize (c, this) &&
		  mathTopAccentAttachment.sanitize (c, this) &&
		  extendedShapeCoverage.sanitize (c, this) &&
		  mathKernInfo.sanitize(c, this));
  }

  inline bool has_math_italics_correction_info (void) const {
    return mathItalicsCorrectionInfo != 0;
  }
  inline const MathItalicsCorrectionInfo&
  get_math_italics_correction_info (void) const {
    return this+mathItalicsCorrectionInfo;
  }

  inline bool has_math_top_accent_attachment (void) const {
    return mathTopAccentAttachment != 0;
  }
  inline const MathTopAccentAttachment&
  get_math_top_accent_attachment (void) const {
    return this+mathTopAccentAttachment;
  }

  inline bool is_extended_shape (hb_codepoint_t glyph) const
  {
    if (likely (extendedShapeCoverage == 0)) return false;
    unsigned int index = (this+extendedShapeCoverage).get_coverage (glyph);
    if (likely (index == NOT_COVERED)) return false;
    return true;
  }

  inline bool has_math_kern_info (void) const { return mathKernInfo != 0; }
  inline const MathKernInfo &get_math_kern_info (void) const {
    return this+mathKernInfo;
  }

protected:
  /* Offset to MathItalicsCorrectionInfo table -
   * from the beginning of MathGlyphInfo table. */
  OffsetTo<MathItalicsCorrectionInfo> mathItalicsCorrectionInfo;

  /* Offset to MathTopAccentAttachment table -
   * from the beginning of MathGlyphInfo table. */
  OffsetTo<MathTopAccentAttachment> mathTopAccentAttachment;

  /* Offset to coverage table for Extended Shape glyphs -
   * from the beginning of MathGlyphInfo table. When the left or right glyph of
   * a box is an extended shape variant, the (ink) box (and not the default
   * position defined by values in MathConstants table) should be used for
   * vertical positioning purposes. May be NULL.. */
  OffsetTo<Coverage> extendedShapeCoverage;

   /* Offset to MathKernInfo table -
    * from the beginning of MathGlyphInfo table. */
  OffsetTo<MathKernInfo> mathKernInfo;

public:
  DEFINE_SIZE_STATIC (8);
};


/*
 * MATH -- The MATH Table
 */

struct MATH
{
  static const hb_tag_t tableTag	= HB_OT_TAG_MATH;

  inline bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (version.sanitize (c) &&
		  likely (version.major == 1) &&
		  mathConstants.sanitize (c, this) &&
		  mathGlyphInfo.sanitize (c, this));
  }

  inline bool has_math_constants (void) const { return mathConstants != 0; }
  inline const MathConstants &get_math_constants (void) const {
    return this+mathConstants;
  }

  inline bool has_math_glyph_info (void) const { return mathGlyphInfo != 0; }
  inline const MathGlyphInfo &get_math_glyph_info (void) const {
    return this+mathGlyphInfo;
  }
protected:
  FixedVersion<>version;		/* Version of the MATH table
					 * initially set to 0x00010000u */
  OffsetTo<MathConstants> mathConstants;/* MathConstants table */
  OffsetTo<MathGlyphInfo> mathGlyphInfo;/* MathGlyphInfo table */

public:
  DEFINE_SIZE_STATIC (8);
};

} /* mathspace OT */


#endif /* HB_OT_LAYOUT_MATH_TABLE_HH */
