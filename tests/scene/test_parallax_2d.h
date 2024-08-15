/**************************************************************************/
/*  test_parallax_2d.h                                                    */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_PARALLAX_2D_H
#define TEST_PARALLAX_2D_H

#include "scene/2d/parallax_2d.h"

#include "tests/test_macros.h"

namespace TestParallax2D {

TEST_CASE("[SceneTree][Parallax2D] Test Scroll Scale") {
    MESSAGE("Scale test");
    Parallax2D parallax;
    Size2 scale(2, 2);
    parallax.set_scroll_scale(scale);
    CHECK(parallax.get_scroll_scale() == scale);
}

TEST_CASE("[SceneTree][Parallax2D] Test Repeat Size") {
    MESSAGE("Size test");
    Parallax2D parallax;
    Size2 size(100, 100);
    parallax.set_repeat_size(size);
    CHECK(parallax.get_repeat_size() == size);
}

TEST_CASE("[SceneTree][Parallax2D] Test Repeat Times") {
    MESSAGE("Times test");
    Parallax2D parallax;
    int times = 5;
    parallax.set_repeat_times(times);
    CHECK(parallax.get_repeat_times() == times);
}

TEST_CASE("[SceneTree][Parallax2D] Test Autoscroll") {
    MESSAGE("Autoscroll test");
    Parallax2D parallax;
    Point2 autoscroll(1, 1);
    parallax.set_autoscroll(autoscroll);
    CHECK(parallax.get_autoscroll() == autoscroll);
}

TEST_CASE("[SceneTree][Parallax2D] Test Scroll Offset") {
    MESSAGE("Offset test");
    Parallax2D parallax;
    Point2 offset(10, 10);
    parallax.set_scroll_offset(offset);
    CHECK(parallax.get_scroll_offset() == offset);
}

TEST_CASE("[SceneTree][Parallax2D] Test Screen Offset") {
    MESSAGE("Screen offset test");
    Parallax2D parallax;
    Point2 offset(20, 20);
    parallax.set_screen_offset(offset);
    CHECK(parallax.get_screen_offset() == offset);
}

TEST_CASE("[SceneTree][Parallax2D] Test Limit Begin") {
    MESSAGE("Limit begin test");
    Parallax2D parallax;
    Point2 limit_begin(-100, -100);
    parallax.set_limit_begin(limit_begin);
    CHECK(parallax.get_limit_begin() == limit_begin);
}

TEST_CASE("[SceneTree][Parallax2D] Test Limit End") {
    MESSAGE("Limit end test");
    Parallax2D parallax;
    Point2 limit_end(100, 100);
    parallax.set_limit_end(limit_end);
    CHECK(parallax.get_limit_end() == limit_end);
}

TEST_CASE("[SceneTree][Parallax2D] Test Follow Viewport") {
    MESSAGE("Follow viewport test");
    Parallax2D parallax;
    parallax.set_follow_viewport(false);
    CHECK(parallax.get_follow_viewport() == false);
}

TEST_CASE("[SceneTree][Parallax2D] Test Ignore Camera Scroll") {
    MESSAGE("Ignore camera scroll test");
    Parallax2D parallax;
    parallax.set_ignore_camera_scroll(true);
    CHECK(parallax.is_ignore_camera_scroll() == true);
}

} // namespace TestParallax2D

#endif // TEST_PARALLAX_2D_H