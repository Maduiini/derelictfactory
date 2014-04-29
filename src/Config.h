
#ifndef H_DER_CONFIG_H
#define H_DER_CONFIG_H

namespace der
{

    class Config
    {
    public:
        Config()
            : m_fullscreen(false)
            , m_screen_w(1280)
            , m_screen_h(720)
            , m_v_sync(false)
        { }

// Some pseudo code to illustrate possible implementation for config reading.
//        bool read_one()
//        {
//            return read_value("fullscreen", m_fullscreen) ||
//                read_value("screen_w", m_screen_w) ||
//                read_value("screen_h", m_screen_h) ||
//                read_value("v_sync", m_v_sync);
//        }
//
//        bool read()
//        {
//            while (true)
//            {
//                skip_empty();
//                if (!read_one())
//                    return false;
//            }
//            return true;
//        }

        bool    m_fullscreen;

        int     m_screen_w;
        int     m_screen_h;
        bool    m_v_sync;
    };

} // der

#endif // H_DER_CONFIG_H

