
#ifndef H_DER_GUI_MANAGER_H
#define H_DER_GUI_MANAGER_H

#include <vector>

namespace der
{
    class Widget;

    class GUIManager
    {
    public:
        GUIManager();
        ~GUIManager();

    private:
        std::vector<Widget*> m_widgets;

    };

} // der

#endif // H_DER_GUI_MANAGER_H
