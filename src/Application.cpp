
#include "Application.h"
#include "Log.h"
#include "Debug.h"

#include "renderer/Mesh.h"
#include "renderer/MeshRenderer.h"
#include "resource/ResourceCache.h"
#include "resource/MeshCache.h"

#include "renderer/Renderer.h"
#include "renderer/SceneRenderer.h"
#include "renderer/ColorFrameBuffer.h"
#include "renderer/PostProcessor.h"
#include "renderer/SSAOEffect.h"
#include "renderer/SunRaysEffect.h"
#include "renderer/DepthOfFieldEffect.h"

#include "scene/Scene.h"
#include "scene/GameObject.h"
#include "scene/Camera.h"

#include "input/CameraController.h"
#include "input/FPSController.h"

#include "ui/GUIManager.h"
#include "ui/GUIRenderer.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "ui/Checkbox.h"
#include "ui/Label.h"
#include "ui/ValueDisplay.h"
#include "ui/TextureDisplay.h"

#include "audio/AudioPlayer.h"

#include <GLFW/glfw3.h>
#include <sstream>

namespace der
{

    void glfw_error_callback(int err, const char * const msg)
    {
        log::error("GLFW: %: %", err, msg);
    }


    Application::Application()
        : m_config()
        , m_window()
        , m_resource_cache()
        , m_post_processor(nullptr)
        , m_scene(nullptr)
        , m_renderer(nullptr)
        , m_scene_renderer(nullptr)
        , m_scene_loader(m_resource_cache)
        , m_current_controller(nullptr)
        , m_fps_controller(nullptr)
        , m_camera_controller(nullptr)
        , m_scene_update_server()
        , m_gui(nullptr)
        , m_gui_renderer(nullptr)
        , m_audio_player(nullptr)
        , m_fps_display(nullptr)
        , m_state_change_display(nullptr)
        , m_vis_objects_display(nullptr)
        , m_nm_display(nullptr)
        , m_nm_slider(nullptr)
        , m_glfw_ready(false)
        , m_ready(false)
        , m_queued_render(true)
    {
        log::set_level(log::LEVEL_TRACE);
//        m_config.read("config.conf");

        ::glfwSetErrorCallback(&glfw_error_callback);
        m_glfw_ready = (::glfwInit() == GL_TRUE);
        if (!m_glfw_ready)
        {
            log::fatal("Could not initialize GLFW");
        }
    }

    Application::~Application()
    {
        delete m_current_controller;
        delete m_renderer;
        delete m_scene_renderer;
        delete m_scene;
        delete m_gui_renderer;
        delete m_gui;
        delete m_audio_player;

        if (m_ready)        m_window.destroy();
        if (m_glfw_ready)   ::glfwTerminate();
    }

    bool Application::init(const char * const title)
    {
        if (m_glfw_ready)
        {
            m_ready = m_window.create(m_config.m_screen_w,
                                      m_config.m_screen_h,
                                      title,
                                      m_config.m_fullscreen);

            if (m_ready)
            {
                m_window.make_current();
                m_ready = m_graphics.init();
                m_window.set_v_sync(m_config.m_v_sync);

                int width, height;
                m_window.get_size(&width, &height);

                m_renderer = new Renderer(&m_graphics, &m_resource_cache);
                m_renderer->set_window_size(width, height);

                m_post_processor = new PostProcessor();
                m_post_processor->resize(width, height);
                m_post_processor->add_effect(new SSAOEffect());
                m_post_processor->add_effect(new SSAOBlurX());
                m_post_processor->add_effect(new SSAOBlurY());
                m_post_processor->add_effect(new SunRaysEffect());
                m_post_processor->add_effect(new DepthOfFieldEffect());

                m_audio_player = new AudioPlayer();
                if (!m_audio_player->init())
                    log::error("Failed to initialize audio player.");
                else
                    m_audio_player->play_music("assets\\music\\outer.ogg", true, 0.0f);
            }
        }
        return is_ready();
    }

    void Application::run()
    {
        if (!is_ready()) return;

        if (!init_scene())
        {
            log::error("Could not initialize scene.");
            return;
        }

        m_scene_update_server.set_scene(m_scene);
        m_scene_update_server.set_cache(&m_resource_cache);

        if (!init_gui())
        {
            log::error("Could not initialize GUI.");
            return;
        }

        double last_time = ::glfwGetTime();

        while (!m_window.should_close())
        {
            double cur_time = ::glfwGetTime();
            double delta_time = cur_time - last_time;
            last_time = cur_time;

            if (m_window.key_pressed(Key::Escape))
                break;

            if (m_window.key_pressed(Key::F5))
                m_resource_cache.refresh_all();
            if (m_window.key_pressed(Key::F6))
                m_resource_cache.reload_all();
            if (m_window.key_pressed(Key::F1))
                m_queued_render = !m_queued_render;

            if (m_window.key_pressed(Key::F2))
                m_scene_renderer->toggle_frustum_culling_enabled();

            if (m_current_controller)
                m_current_controller->update(delta_time);

            m_scene->update(&m_resource_cache);

            m_scene_renderer->set_time(cur_time);
            render();

            update_title(cur_time);

            m_window.poll_events();
            if (m_window.has_resized())
            {
                int w = 0, h = 0;
                m_window.get_size(&w, &h);
                m_graphics.set_viewport(0, 0, w, h);
                m_scene->reshape(w, h);
                m_post_processor->resize(w, h);
                m_renderer->set_window_size(w, h);
            }

            m_gui->update(delta_time,
                          Vector2(m_window.get_mouse_x(), m_window.get_mouse_y()),
                          m_window.button_down(MouseButton::Left));
            m_gui_renderer->set_visible(!m_window.is_mouse_captured());

            m_scene_update_server.pump();
        }
    }

    bool Application::is_ready() const
    { return m_glfw_ready && m_ready; }

    void Application::set_queued_render_enabled(bool enabled)
    { m_queued_render = enabled; }

    bool Application::is_queued_render_enabled() const
    { return m_queued_render; }

    bool Application::init_scene()
    {
        m_scene = new Scene();
        m_scene_renderer = new SceneRenderer(m_scene, &m_resource_cache);

        // Load the test scene
//        if (m_scene_loader.load("test_scene.derscene", m_scene))
        if (m_scene_loader.load("derelict_factory.derscene", m_scene))
            log::info("Scene loaded");
        else
        {
            log::error("Could not load scene");
            return false;
        }

        // Create camera
        GameObject *camera_object = m_scene->new_object();
        camera_object->set_camera(new Camera());
        camera_object->set_position(Vector3(0.0f, 1.8f, 0.0f));
        m_scene->set_camera_object(camera_object->getID());

        m_camera_controller = new CameraController();
        m_camera_controller->set_window(&m_window);
        m_camera_controller->set_object(camera_object);

        m_fps_controller = new FPSController();
        m_fps_controller->set_window(&m_window);
        m_fps_controller->set_object(camera_object);

        m_current_controller = m_camera_controller;

        return true;
    }


    template <class T>
    class CheckboxForwarder : public GUIEventHandler
    {
    public:
        typedef void (T::*MethodT)(bool);

        T *     m_object;
        MethodT m_method;

        CheckboxForwarder(T *object, MethodT method)
            : m_object(object)
            , m_method(method)
        { }

        virtual void handle(Widget *widget) override
        {
            const Checkbox *cb = reinterpret_cast<Checkbox*>(widget);
            (m_object->*m_method)(cb->is_checked());
        }
    };

    class NormalMapSliderChanged : public GUIEventHandler
    {
    public:
        SceneRenderer *m_scene_renderer;
        ValueDisplay *m_nm_display;

        NormalMapSliderChanged(SceneRenderer *renderer, ValueDisplay *nm_display)
            : m_scene_renderer(renderer), m_nm_display(nm_display)
        { }

        virtual void handle(Widget *widget) override
        {
            const Slider *slider = reinterpret_cast<Slider*>(widget);
            const float value = slider->get_absolute_value();
            m_scene_renderer->set_normalmap_influence(value);
            const float rounded_value = int(value * 100.0f) / 100.0f;
            m_nm_display->set_value(rounded_value);
        }
    };

    class SunSliderChanged : public GUIEventHandler
    {
    public:
        Scene *m_scene;
        ValueDisplay *m_display;

        SunSliderChanged(Scene *scene, ValueDisplay *display)
            : m_scene(scene), m_display(display)
        { }

        virtual void handle(Widget *widget) override
        {
            const Slider *slider = reinterpret_cast<Slider*>(widget);
            const float value = slider->get_absolute_value();
            const float rounded_value = int(value * 100.0f) / 100.0f;
            m_display->set_value(rounded_value);

//            Quaternion orientation1;
//            orientation1.from_axis_angle(Vector3(-0.3, -0.4, 0.8).normalized(), Math::DEG_2_RAD * 180.0f);
//            Quaternion orientation2;
//            orientation1.from_axis_angle(Vector3(-0.3, -0.4, 0.8).normalized(), Math::DEG_2_RAD * 140.0f);
//
//            Quaternion sun_orientation = slerp(orientation1, orientation2, value);

            const float theta = lerp(0.0f, 360.0f, value) * Math::DEG_2_RAD;
            const Vector3 axis1(0.0, 0.0, 1.0);
            const Vector3 axis2(1.0, 1.0, 1.0);
            const Vector3 axis = lerp(axis1, axis2, value).normalized();

            GameObject *sun = m_scene->get_sun();
            sun->set_rotation(axis, theta); //sun_orientation);
        }
    };

    class SwitchControlPressed : public GUIEventHandler
    {
    public:
        Application *m_application;
        bool m_make_fps;

        SwitchControlPressed(Application *app)
            : m_application(app)
            , m_make_fps(true)
        { }

        virtual void handle(Widget *widget) override
        {
            m_application->set_controller(m_make_fps);
            m_make_fps = !m_make_fps;
        }

    };

    class ToggleMusic : public GUIEventHandler
    {
    public:
        AudioPlayer *m_audio_player;

        ToggleMusic(AudioPlayer *player)
            : m_audio_player(player)
        { }

        virtual void handle(Widget *widget) override
        {
            const Checkbox *cb = reinterpret_cast<Checkbox*>(widget);
            m_audio_player->set_music_volume(cb->is_checked() ? 0.5f : 0.0f);
        }
    };

    void Application::set_controller(bool set_fps)
    {
        if (set_fps)
        {
            m_current_controller = m_fps_controller;
            m_fps_controller->set_rotation_x(m_camera_controller->get_rotation_x());
            m_fps_controller->set_rotation_y(m_camera_controller->get_rotation_y());
            Vector3 pos = m_fps_controller->get_object()->get_position();
            m_fps_controller->set_jump_y(pos.y - FPSController::eye_level);
            m_fps_controller->set_speed(Vector2::zero);
            m_fps_controller->set_jump_velocity(0.0f);
        }
        else
        {
            m_current_controller = m_camera_controller;
            m_camera_controller->set_rotation_x(m_fps_controller->get_rotation_x());
            m_camera_controller->set_rotation_y(m_fps_controller->get_rotation_y());
        }
    }

    bool Application::init_gui()
    {
        m_gui = new GUIManager();
        m_gui_renderer = new GUIRenderer(m_gui, &m_window);

        if (!m_gui_renderer->init())
        {
            log::error("Failed to initialize GUIRenderer.");
            return false;
        }

        m_fps_display = new ValueDisplay(Vector2(15, 10), "FPS: ");
        m_gui->add_widget(m_fps_display);

        m_state_change_display = new ValueDisplay(Vector2(15, 40), "State changes: ");
        m_gui->add_widget(m_state_change_display);

        m_vis_objects_display = new ValueDisplay(Vector2(15, 70), "Visible objects: ");
        m_gui->add_widget(m_vis_objects_display);

        m_nm_display = new ValueDisplay(Vector2(15, 120), "Normalmap influence: ");
        m_gui->add_widget(m_nm_display);
        m_nm_slider = new Slider(Vector2(15, 160), 360.0f, 0.0f, 2.0);
        m_nm_slider->set_value_changed_handler(new NormalMapSliderChanged(m_scene_renderer, m_nm_display));
        m_nm_slider->set_value(1.0f);
        m_gui->add_widget(m_nm_slider);

        Checkbox *render_queue_box = new Checkbox(Vector2(15, 200), "Use render queue");
        render_queue_box->set_state_changed_handler(
            new CheckboxForwarder<Application>(this, &Application::set_queued_render_enabled));
        render_queue_box->set_checked(true);
        m_gui->add_widget(render_queue_box);

        Checkbox *frustum_culling_box = new Checkbox(Vector2(15, 240), "Frustum culling");
        frustum_culling_box->set_state_changed_handler(
            new CheckboxForwarder<SceneRenderer>(m_scene_renderer, &SceneRenderer::set_frustum_culling_enabled));
        frustum_culling_box->set_checked(true);
        m_gui->add_widget(frustum_culling_box);

        Checkbox *debug_draw_box = new Checkbox(Vector2(15, 280), "Debug draw");
        debug_draw_box->set_state_changed_handler(
            new CheckboxForwarder<SceneRenderer>(m_scene_renderer, &SceneRenderer::set_debug_draw_enabled));
        debug_draw_box->set_checked(false);
        m_gui->add_widget(debug_draw_box);

        Checkbox *post_processing_box = new Checkbox(Vector2(15, 320), "Post-processing");
        post_processing_box->set_state_changed_handler(
            new CheckboxForwarder<PostProcessor>(m_post_processor, &PostProcessor::set_enabled));
        post_processing_box->set_checked(false);
        m_gui->add_widget(post_processing_box);

        Checkbox *shadow_mapping_box = new Checkbox(Vector2(15, 360), "Shadow mapping");
        shadow_mapping_box->set_state_changed_handler(
            new CheckboxForwarder<SceneRenderer>(m_scene_renderer, &SceneRenderer::set_shadow_map_enabled));
        shadow_mapping_box->set_checked(true);
        m_gui->add_widget(shadow_mapping_box);

        ValueDisplay *sun_display = new ValueDisplay(Vector2(15, 420), "Sun orientation: ");
        m_gui->add_widget(sun_display);
        Slider *m_sun_slider = new Slider(Vector2(15, 460), 360.0f, 0.0f, 1.0);
        m_sun_slider->set_value_changed_handler(new SunSliderChanged(m_scene, sun_display));
        m_sun_slider->set_value(0.0f);
        m_gui->add_widget(m_sun_slider);

        Button *control_switch_button = new Button(Vector2(15, 500), Vector2(200, 45), "Switch control");
        control_switch_button->set_released_handler(new SwitchControlPressed(this));
        m_gui->add_widget(control_switch_button);

        Checkbox *dof_debug_box = new Checkbox(Vector2(280, 320), "Debug DoF");
        dof_debug_box->set_state_changed_handler(
            new CheckboxForwarder<DepthOfFieldEffect>(static_cast<DepthOfFieldEffect*>(m_post_processor->get_effect(4)), &DepthOfFieldEffect::enable_debugging));
        dof_debug_box->set_checked(false);
        m_gui->add_widget(dof_debug_box);

        Checkbox *music_toggle_box = new Checkbox(Vector2(280, 360), "Toggle Music");
        music_toggle_box->set_state_changed_handler(new ToggleMusic(m_audio_player));
        music_toggle_box->set_checked(false);
        m_gui->add_widget(music_toggle_box);

        return true;
    }

    void Application::render()
    {
        m_graphics.reset_state_changes();

        int w, h;
        m_window.get_size(&w, &h);

        m_scene_renderer->update_shadowmap(m_renderer, &m_graphics);
        FrameBuffer::set_viewport(w, h);

        m_post_processor->begin_scene();
        m_graphics.clear();

        if (m_queued_render)
            m_scene_renderer->render(m_renderer);
        else
            m_scene_renderer->render_immediate(m_renderer);

        m_post_processor->post_process(&m_graphics, m_resource_cache, w, h);

        m_gui_renderer->render(&m_graphics, m_resource_cache);

        m_window.swap_buffer();
    }


    void Application::update_title(double current_time)
    {
        static double last_time = current_time;
        static size_t frames = 0;

        if (current_time - last_time >= 0.5)
        {
            std::string title = "derelict fps: ";

            const double fps = double(frames) / (current_time - last_time);
            title += to_string(fps);
            title += ", state changes: " + to_string(m_graphics.get_state_changes());

            title += ", visible objects: " + to_string(m_scene_renderer->get_visible_object_count());

            m_window.set_title(title.c_str());

            m_fps_display->set_value(int(fps));
            m_state_change_display->set_value(m_graphics.get_state_changes());
            m_vis_objects_display->set_value(m_scene_renderer->get_visible_object_count());

            last_time = current_time;
            frames = 0;
        }
        frames++;
    }


} // der
