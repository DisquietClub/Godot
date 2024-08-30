/**************************************************************************/
/*  tool_button_editor_plugin.cpp                                         */
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

#include "tool_button_editor_plugin.h"

#include "editor/editor_property_name_processor.h"
#include "editor/editor_undo_redo_manager.h"
#include "scene/gui/button.h"

bool ToolButtonInspectorPlugin::can_handle(Object *p_object) {
	Ref<Script> scr = p_object->get_script();
	return scr.is_valid() && scr->is_tool();
}

void ToolButtonInspectorPlugin::update_action_icon(Button *p_action_button) {
	p_action_button->set_icon(p_action_button->get_editor_theme_icon(action_icon));
}

void ToolButtonInspectorPlugin::call_action(Object *p_object, const StringName &p_method_name) {
	bool method_is_valid = false;
	int method_arg_count = p_object->get_method_argument_count(p_method_name, &method_is_valid);

	ERR_FAIL_COND_MSG(!method_is_valid, vformat("Tool button method is invalid. Could not find method '%s' on %s.", p_method_name, p_object->get_class_name()));

	Variant undo_redo = EditorUndoRedoManager::get_singleton();

	const Variant *args = nullptr;
	int argc = 0;

	// If the function takes arguments the first argument is always the EditorUndoRedoManager.
	if (method_arg_count != 0) {
		args = { &undo_redo };
		argc = 1;
	}

	Callable::CallError ce;
	p_object->callp(p_method_name, &args, argc, ce);
	ERR_FAIL_COND_MSG(ce.error != Callable::CallError::CALL_OK, vformat("Error calling tool button method on %s: %s.", p_object->get_class_name(), Variant::get_call_error_text(p_method_name, &args, argc, ce)));
}

bool ToolButtonInspectorPlugin::parse_property(Object *p_object, const Variant::Type p_type, const String &p_path, const PropertyHint p_hint, const String &p_hint_text, const BitField<PropertyUsageFlags> p_usage, const bool p_wide) {
	if (p_type != Variant::CALLABLE || !p_usage.has_flag(PROPERTY_USAGE_EDITOR)) {
		return false;
	}

	const PackedStringArray splits = p_hint_text.split(",");
	ERR_FAIL_COND_V_MSG(splits.size() < 1, false, "Tool button annotations require a method to call.");
	const String &method = splits[0];
	const String &hint_text = splits.size() > 1 ? splits[1] : "";
	const String &hint_icon = splits.size() > 2 ? splits[2] : "Callable";

	String action_text = hint_text;
	if (action_text.is_empty()) {
		action_text = EditorPropertyNameProcessor::get_singleton()->process_name(method, EditorPropertyNameProcessor::STYLE_CAPITALIZED);
	}

	action_icon = hint_icon;

	Button *action_button = EditorInspector::create_inspector_action_button(action_text);
	action_button->connect(SceneStringName(theme_changed), callable_mp(this, &ToolButtonInspectorPlugin::update_action_icon).bind(action_button));
	action_button->connect(SceneStringName(pressed), callable_mp(this, &ToolButtonInspectorPlugin::call_action).bind(p_object, method));

	add_custom_control(action_button);
	return true;
}

ToolButtonEditorPlugin::ToolButtonEditorPlugin() {
	Ref<ToolButtonInspectorPlugin> plugin;
	plugin.instantiate();
	add_inspector_plugin(plugin);
}
