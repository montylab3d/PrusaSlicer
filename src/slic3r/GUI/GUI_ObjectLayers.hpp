#ifndef slic3r_GUI_ObjectLayers_hpp_
#define slic3r_GUI_ObjectLayers_hpp_

#include "GUI_ObjectSettings.hpp"
#include "wxExtensions.hpp"

#ifdef __WXOSX__
#include "../libslic3r/PrintConfig.hpp"
#endif

class wxBoxSizer;

namespace Slic3r {
class ModelObject;

namespace GUI {
class ConfigOptionsGroup;

typedef double                          coordf_t;
typedef std::pair<coordf_t, coordf_t>   t_layer_height_range;

class ObjectLayers;

enum EditorType
{
    etUndef         = 0,
    etMinZ          = 1,
    etMaxZ          = 2,
    etLayerHeight   = 4,
};

class LayerRangeEditor : public wxTextCtrl
{
    bool                m_enter_pressed     { false };
    bool                m_call_kill_focus   { false };
    wxString            m_valid_value;
    EditorType          m_type;

    std::function<void(EditorType)> m_set_focus_data;

public:
    LayerRangeEditor(   ObjectLayers* parent,
                        const wxString& value = wxEmptyString,
                        EditorType type = etUndef,
                        std::function<void(EditorType)>     set_focus_data_fn   = [](EditorType)      {;},
                        std::function<bool(coordf_t, bool)> edit_fn             = [](coordf_t, bool) {return false; }
                        );
    ~LayerRangeEditor() {}

    EditorType          type() const {return m_type;}
    void                set_focus_data() const { m_set_focus_data(m_type);}
    void                msw_rescale();

private:
    coordf_t            get_value();
};

enum ButtonType
{
    btUndef         = 0,
    btAdd           = 1,
    btDelete        = 2,
};

class LayerRangeButton : public ScalableButton
{
    ButtonType              m_type;
    t_layer_height_range    m_range;

public:
    LayerRangeButton(   wxWindow* parent,
                        const ScalableBitmap& bitmap,
                        t_layer_height_range range,
                        ButtonType type
                        ):
    m_range(range), m_type(type), 
    ScalableButton(parent, wxID_ANY, bitmap) {}

    ~LayerRangeButton() {}

    ButtonType                      type()  const { return m_type; }
    const t_layer_height_range&     range() const { return m_range; }
};

class ObjectLayers : public OG_Settings
{
    ScalableBitmap  m_bmp_delete;
    ScalableBitmap  m_bmp_add;
    ModelObject*    m_object {nullptr};

    wxFlexGridSizer*       m_grid_sizer;
    t_layer_height_range   m_selectable_range;
    EditorType             m_selection_type {etUndef};
    ButtonType             m_pushed_btn_type {btUndef};
    t_layer_height_range   m_pushed_btn_range;

public:
    ObjectLayers(wxWindow* parent);
    ~ObjectLayers() {}

    void        select_editor(LayerRangeEditor* editor, const bool is_last_edited_range);
    wxSizer*    create_layer(const t_layer_height_range& range);    // without_buttons
    void        create_layers_list();
    void        update_layers_list();

    void        update_scene_from_editor_selection() const;

    void        UpdateAndShow(const bool show) override;
    void        msw_rescale();
    void        reset_selection();
    void        add_range(t_layer_height_range range);
    void        delete_range(t_layer_height_range range);
    void        set_selectable_range(const t_layer_height_range& range) { m_selectable_range = range; }
    void        set_pushed_button_items(ButtonType type, const t_layer_height_range& range);
    void        reset_pushed_button_items();
    void        process_btn_pushed();
    ButtonType  pushed_btn_type() const { return m_pushed_btn_type; }

    friend class LayerRangeEditor;
};

}}

#endif // slic3r_GUI_ObjectLayers_hpp_
