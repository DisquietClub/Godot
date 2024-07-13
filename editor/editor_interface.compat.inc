/**************************************************************************/
/*  editor_interface.compat.inc                                           */
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

#include "editor_interface.h"

#include "editor/gui/scene_tree_editor.h"
#include "editor/property_selector.h"

#ifndef DISABLE_DEPRECATED

void EditorInterface::popup_node_selector_compat_94322(const Callable &p_callback, const TypedArray<StringName> &p_valid_types) {
	// TODO: Should reuse dialog instance instead of creating a fresh one, but need to rework set_valid_types first.
	if (node_selector) {
		node_selector->disconnect(SNAME("selected"), callable_mp(this, &EditorInterface::_node_selected).bind(p_callback));
		node_selector->disconnect(SNAME("canceled"), callable_mp(this, &EditorInterface::_node_selection_canceled).bind(p_callback));
		get_base_control()->remove_child(node_selector);
		node_selector->queue_free();
	}
	node_selector = memnew(SceneTreeDialog);

	Vector<StringName> valid_types;
	int length = p_valid_types.size();
	valid_types.resize(length);
	for (int i = 0; i < length; i++) {
		valid_types.write[i] = p_valid_types[i];
	}
	node_selector->set_valid_types(valid_types);

	get_base_control()->add_child(node_selector);

	node_selector->popup_scenetree_dialog();

	const Callable selected_callback = callable_mp(this, &EditorInterface::_node_selected).bind(p_callback);
	node_selector->connect(SNAME("selected"), selected_callback, CONNECT_DEFERRED);

	const Callable canceled_callback = callable_mp(this, &EditorInterface::_node_selection_canceled).bind(p_callback);
	node_selector->connect(SNAME("canceled"), canceled_callback, CONNECT_DEFERRED);
}

void EditorInterface::popup_property_selector_compat_94322(Object *p_object, const Callable &p_callback, const PackedInt32Array &p_type_filter) {
	// TODO: Should reuse dialog instance instead of creating a fresh one, but need to rework set_type_filter first.
	if (property_selector) {
		property_selector->disconnect(SNAME("selected"), callable_mp(this, &EditorInterface::_property_selected).bind(p_callback));
		property_selector->disconnect(SNAME("canceled"), callable_mp(this, &EditorInterface::_property_selection_canceled).bind(p_callback));
		get_base_control()->remove_child(property_selector);
		property_selector->queue_free();
	}
	property_selector = memnew(PropertySelector);

	Vector<Variant::Type> type_filter;
	int length = p_type_filter.size();
	type_filter.resize(length);
	for (int i = 0; i < length; i++) {
		type_filter.write[i] = (Variant::Type)p_type_filter[i];
	}
	property_selector->set_type_filter(type_filter);

	get_base_control()->add_child(property_selector);

	property_selector->select_property_from_instance(p_object);

	const Callable selected_callback = callable_mp(this, &EditorInterface::_property_selected).bind(p_callback);
	property_selector->connect(SNAME("selected"), selected_callback, CONNECT_DEFERRED);

	const Callable canceled_callback = callable_mp(this, &EditorInterface::_property_selection_canceled).bind(p_callback);
	property_selector->connect(SNAME("canceled"), canceled_callback, CONNECT_DEFERRED);
}

void EditorInterface::_bind_compatibility_methods() {
	ClassDB::bind_compatibility_method(D_METHOD("popup_node_selector", "callback", "valid_types"), &EditorInterface::popup_node_selector_compat_94322);
	ClassDB::bind_compatibility_method(D_METHOD("popup_property_selector", "object", "callback", "type_filter"), &EditorInterface::popup_property_selector_compat_94322);
}

#endif
