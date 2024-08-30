/**************************************************************************/
/*  configuration_info_editor_plugin.cpp                                  */
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

#include "configuration_info_editor_plugin.h"

#include "editor/editor_configuration_info.h"
#include "scene/gui/grid_container.h"

// Inspector controls.

void ConfigurationInfoList::_update_content() {
	if (!object) {
		hide();
		return;
	}

	Array config_info_dicts = EditorConfigurationInfo::get_configuration_info_dicts(object);
	if (config_info_dicts.is_empty()) {
		hide();
		return;
	}

	title_label->set_text(vformat(TTRN("%d Configuration Info", "%d Configuration Infos", config_info_dicts.size()), config_info_dicts.size()));

	config_info_list->clear();
	for (int i = 0; i < config_info_dicts.size(); i++) {
		Dictionary config_info = config_info_dicts[i];
		String text = EditorConfigurationInfo::format_dict_as_string(config_info, false, true);
		StringName icon = EditorConfigurationInfo::get_severity_icon(config_info.get("severity", "warning"));
		config_info_list->add_item(text, get_editor_theme_icon(icon));
	}

	bg_panel->add_theme_style_override(SceneStringName(panel), get_theme_stylebox(SNAME("bg_group_note"), SNAME("EditorProperty")));
	show();
}

void ConfigurationInfoList::_update_toggler() {
	Ref<Texture2D> arrow;
	if (config_info_list->is_visible()) {
		arrow = get_theme_icon(SNAME("arrow"), SNAME("Tree"));
		set_tooltip_text(TTR("Collapse configuration info."));
	} else {
		if (is_layout_rtl()) {
			arrow = get_theme_icon(SNAME("arrow_collapsed"), SNAME("Tree"));
		} else {
			arrow = get_theme_icon(SNAME("arrow_collapsed_mirrored"), SNAME("Tree"));
		}
		set_tooltip_text(TTR("Expand configuration info."));
	}

	expand_icon->set_texture(arrow);
}

void ConfigurationInfoList::set_object(Object *p_object) {
	object = p_object;
	_update_content();
}

void ConfigurationInfoList::gui_input(const Ref<InputEvent> &p_event) {
	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid() && mb->is_pressed() && mb->get_button_index() == MouseButton::LEFT) {
		bool state = !config_info_list->is_visible();

		config_info_list->set_visible(state);
		list_filler_right->set_visible(state);

		_update_toggler();
	}
}

void ConfigurationInfoList::_notification(int p_notification) {
	switch (p_notification) {
		case NOTIFICATION_ENTER_TREE:
		case NOTIFICATION_THEME_CHANGED:
			_update_content();
			_update_toggler();
			break;
	}
}

ConfigurationInfoList::ConfigurationInfoList() {
	set_mouse_filter(MOUSE_FILTER_STOP);
	hide();

	bg_panel = memnew(PanelContainer);
	bg_panel->set_mouse_filter(MOUSE_FILTER_IGNORE);
	add_child(bg_panel);

	grid = memnew(GridContainer);
	grid->set_columns(2);
	bg_panel->add_child(grid);

	title_label = memnew(Label);
	title_label->set_autowrap_mode(TextServer::AutowrapMode::AUTOWRAP_WORD);
	title_label->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	title_label->set_vertical_alignment(VerticalAlignment::VERTICAL_ALIGNMENT_CENTER);
	grid->add_child(title_label);

	expand_icon = memnew(TextureRect);
	expand_icon->set_stretch_mode(TextureRect::StretchMode::STRETCH_KEEP_CENTERED);
	grid->add_child(expand_icon);

	config_info_list = memnew(ItemList);
	config_info_list->set_allow_search(false);
	config_info_list->set_auto_height(true);
	config_info_list->set_h_size_flags(Control::SIZE_EXPAND_FILL);
	grid->add_child(config_info_list);

	// Filler.
	list_filler_right = memnew(Control);
	grid->add_child(list_filler_right);
}

bool EditorInspectorPluginConfigurationInfo::can_handle(Object *p_object) {
	return Object::cast_to<Node>(p_object) != nullptr || Object::cast_to<Resource>(p_object) != nullptr;
}

void EditorInspectorPluginConfigurationInfo::parse_begin(Object *p_object) {
	ConfigurationInfoList *config_info_list = memnew(ConfigurationInfoList);
	config_info_list->set_object(p_object);
	add_custom_control(config_info_list);
}

// Editor plugin.

ConfigurationInfoEditorPlugin::ConfigurationInfoEditorPlugin() {
	Ref<EditorInspectorPluginConfigurationInfo> plugin;
	plugin.instantiate();
	add_inspector_plugin(plugin);
}
