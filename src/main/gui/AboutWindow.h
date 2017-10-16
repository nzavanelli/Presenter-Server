/*
 * AboutWindow.h
 *
 *  Created on: 13.08.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ABOUTWINDOW_H_
#define SRC_MAIN_GUI_ABOUTWINDOW_H_

#include <QDialog>

namespace Ui {
    class AboutWindow;
}

/**
 * Represents a display that show some information about the presenter server.
 */
class AboutWindow : public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window
         */
        AboutWindow(QWidget* parent);

        /**
         * Destructor. Deletes the ui object.
         */
        virtual ~AboutWindow();

    private:
        /**
         * The ui of the dialog.
         */
        Ui::AboutWindow* ui;

        /**
         * Returns the compiler used for compilation.
         *
         * @return The compiler name
         */
        QString getCompiler();

        /**
         * Returns the compiler flags used for compilation.
         *
         * @return The compiler flags if available
         */
        QString getCompilerFlags();
};

#endif // SRC_MAIN_GUI_ABOUTWINDOW_H_

