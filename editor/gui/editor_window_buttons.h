/**************************************************************************/
/*  editor_window_buttons.h                                               */
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

#ifndef EDITOR_WINDOW_BUTTONS_H
#define EDITOR_WINDOW_BUTTONS_H

#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/main/window.h"

class EditorWindowButtons : public HBoxContainer {
	GDCLASS(EditorWindowButtons, HBoxContainer);

	Button *button_minimize = nullptr;
	Button *button_maximize = nullptr;
	Button *button_close = nullptr;
	Window *window = nullptr;

	void _minimize();
	void _maximize();
	void _close();
	void _resized();
	void _update_buttons();

protected:
	void _notification(int p_what);

public:
	void set_can_move_window(bool p_enabled);
	bool get_can_move_window() const;

	EditorWindowButtons();
};

#endif // EDITOR_WINDOW_BUTTONS_H
