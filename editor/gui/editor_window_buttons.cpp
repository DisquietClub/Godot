/**************************************************************************/
/*  editor_window_buttons.cpp                                             */
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

#include "editor_window_buttons.h"
#include "editor/editor_node.h"

void EditorWindowButtons::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_READY: {
			window = Object::cast_to<Window>(get_viewport());
			ERR_FAIL_NULL(window);

			// The theme icons are not available in the EditorNode constructor,
			// we need to setup them on ready.
			button_minimize->set_theme_type_variation("FlatMenuButton");
			button_minimize->set_icon(get_editor_theme_icon(SNAME("Minimize")));
			button_minimize->set_tooltip_text(TTR("Minimize"));

			button_maximize->set_theme_type_variation("FlatMenuButton");

			button_close->set_theme_type_variation("FlatMenuButton");
			button_close->set_icon(get_editor_theme_icon(SNAME("Close")));
			button_close->set_tooltip_text(TTR("Close"));

			_update_buttons();

			get_viewport()->connect("size_changed", callable_mp(this, &EditorWindowButtons::_resized));
		} break;
	}
}

void EditorWindowButtons::_update_buttons() {
	ERR_FAIL_NULL(window);

	if (window->get_mode() == Window::MODE_MAXIMIZED) {
		button_maximize->set_icon(get_editor_theme_icon(SNAME("Restore")));
		button_maximize->set_tooltip_text(TTR("Restore"));
	} else {
		button_maximize->set_icon(get_editor_theme_icon(SNAME("Maximize")));
		button_maximize->set_tooltip_text(TTR("Maximize"));
	}
}

void EditorWindowButtons::_minimize() {
	ERR_FAIL_NULL(window);

	window->set_mode(Window::MODE_MINIMIZED);
}

void EditorWindowButtons::_maximize() {
	ERR_FAIL_NULL(window);

	if (window->get_mode() == Window::MODE_WINDOWED) {
		window->set_mode(Window::MODE_MAXIMIZED);
	} else {
		window->set_mode(Window::MODE_WINDOWED);
	}
}

void EditorWindowButtons::_close() {
	EditorNode::get_singleton()->exit();
}

void EditorWindowButtons::_resized() {
	_update_buttons();
}

EditorWindowButtons::EditorWindowButtons() {
	button_minimize = memnew(Button);
	button_minimize->set_focus_mode(Control::FOCUS_NONE);
	button_minimize->set_icon(get_editor_theme_icon(SNAME("Minimize")));
	button_minimize->connect(SceneStringName(pressed), callable_mp(this, &EditorWindowButtons::_minimize));
	add_child(button_minimize);

	button_maximize = memnew(Button);
	button_maximize->set_focus_mode(Control::FOCUS_NONE);
	button_maximize->set_icon(get_editor_theme_icon(SNAME("Maximize")));
	button_maximize->connect(SceneStringName(pressed), callable_mp(this, &EditorWindowButtons::_maximize));
	add_child(button_maximize);

	button_close = memnew(Button);
	button_close->set_focus_mode(Control::FOCUS_NONE);
	button_close->set_icon(get_editor_theme_icon(SNAME("Close")));
	button_close->connect(SceneStringName(pressed), callable_mp(this, &EditorWindowButtons::_close));
	add_child(button_close);
}
