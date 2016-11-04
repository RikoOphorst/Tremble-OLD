/********************************************************************************
** Form generated from reading UI file 'config_design.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CONFIG_DESIGN_H
#define CONFIG_DESIGN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SetupDialog
{
public:
    QWidget *root;
    QWidget *formLayoutWidget;
    QFormLayout *form_layout;
    QLabel *label_fullscreen;
    QCheckBox *is_fullscreen;
    QLabel *label_gpu;
    QComboBox *selection_gpu;
    QLabel *label_window_resolution;
    QComboBox *selection_window_resolution;
    QLabel *label_render_resolution;
    QComboBox *selection_render_resolution;
    QLabel *label_mouse_lock;
    QCheckBox *is_mouse_lock;
    QLabel *label_scene;
    QComboBox *selection_scene;
    QLabel *label_free_camera;
    QCheckBox *is_free_camera;
    QLabel *label_frustum_culling;
    QCheckBox *is_frustum_cullling;
    QDialogButtonBox *launch_box;

    void setupUi(QMainWindow *SetupDialog)
    {
        if (SetupDialog->objectName().isEmpty())
            SetupDialog->setObjectName(QStringLiteral("SetupDialog"));
        SetupDialog->resize(461, 300);
        SetupDialog->setMinimumSize(QSize(461, 0));
        SetupDialog->setMaximumSize(QSize(461, 300));
        SetupDialog->setBaseSize(QSize(461, 300));
        QFont font;
        font.setPointSize(8);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        font.setStyleStrategy(QFont::PreferDefault);
        SetupDialog->setFont(font);
        root = new QWidget(SetupDialog);
        root->setObjectName(QStringLiteral("root"));
        formLayoutWidget = new QWidget(root);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 461, 296));
        form_layout = new QFormLayout(formLayoutWidget);
        form_layout->setObjectName(QStringLiteral("form_layout"));
        form_layout->setRowWrapPolicy(QFormLayout::WrapLongRows);
        form_layout->setHorizontalSpacing(70);
        form_layout->setVerticalSpacing(15);
        form_layout->setContentsMargins(20, 20, 20, 0);
        label_fullscreen = new QLabel(formLayoutWidget);
        label_fullscreen->setObjectName(QStringLiteral("label_fullscreen"));

        form_layout->setWidget(0, QFormLayout::LabelRole, label_fullscreen);

        is_fullscreen = new QCheckBox(formLayoutWidget);
        is_fullscreen->setObjectName(QStringLiteral("is_fullscreen"));

        form_layout->setWidget(0, QFormLayout::FieldRole, is_fullscreen);

        label_gpu = new QLabel(formLayoutWidget);
        label_gpu->setObjectName(QStringLiteral("label_gpu"));

        form_layout->setWidget(1, QFormLayout::LabelRole, label_gpu);

        selection_gpu = new QComboBox(formLayoutWidget);
        selection_gpu->setObjectName(QStringLiteral("selection_gpu"));

        form_layout->setWidget(1, QFormLayout::FieldRole, selection_gpu);

        label_window_resolution = new QLabel(formLayoutWidget);
        label_window_resolution->setObjectName(QStringLiteral("label_window_resolution"));

        form_layout->setWidget(2, QFormLayout::LabelRole, label_window_resolution);

        selection_window_resolution = new QComboBox(formLayoutWidget);
        selection_window_resolution->setObjectName(QStringLiteral("selection_window_resolution"));
        selection_window_resolution->setEnabled(true);

        form_layout->setWidget(2, QFormLayout::FieldRole, selection_window_resolution);

        label_render_resolution = new QLabel(formLayoutWidget);
        label_render_resolution->setObjectName(QStringLiteral("label_render_resolution"));

        form_layout->setWidget(3, QFormLayout::LabelRole, label_render_resolution);

        selection_render_resolution = new QComboBox(formLayoutWidget);
        selection_render_resolution->setObjectName(QStringLiteral("selection_render_resolution"));

        form_layout->setWidget(3, QFormLayout::FieldRole, selection_render_resolution);

        label_mouse_lock = new QLabel(formLayoutWidget);
        label_mouse_lock->setObjectName(QStringLiteral("label_mouse_lock"));

        form_layout->setWidget(4, QFormLayout::LabelRole, label_mouse_lock);

        is_mouse_lock = new QCheckBox(formLayoutWidget);
        is_mouse_lock->setObjectName(QStringLiteral("is_mouse_lock"));

        form_layout->setWidget(4, QFormLayout::FieldRole, is_mouse_lock);

        label_scene = new QLabel(formLayoutWidget);
        label_scene->setObjectName(QStringLiteral("label_scene"));

        form_layout->setWidget(5, QFormLayout::LabelRole, label_scene);

        selection_scene = new QComboBox(formLayoutWidget);
        selection_scene->setObjectName(QStringLiteral("selection_scene"));

        form_layout->setWidget(5, QFormLayout::FieldRole, selection_scene);

        label_free_camera = new QLabel(formLayoutWidget);
        label_free_camera->setObjectName(QStringLiteral("label_free_camera"));

        form_layout->setWidget(6, QFormLayout::LabelRole, label_free_camera);

        is_free_camera = new QCheckBox(formLayoutWidget);
        is_free_camera->setObjectName(QStringLiteral("is_free_camera"));

        form_layout->setWidget(6, QFormLayout::FieldRole, is_free_camera);

        label_frustum_culling = new QLabel(formLayoutWidget);
        label_frustum_culling->setObjectName(QStringLiteral("label_frustum_culling"));

        form_layout->setWidget(7, QFormLayout::LabelRole, label_frustum_culling);

        is_frustum_cullling = new QCheckBox(formLayoutWidget);
        is_frustum_cullling->setObjectName(QStringLiteral("is_frustum_cullling"));
        is_frustum_cullling->setEnabled(true);
        is_frustum_cullling->setChecked(true);
        is_frustum_cullling->setAutoRepeat(false);

        form_layout->setWidget(7, QFormLayout::FieldRole, is_frustum_cullling);

        launch_box = new QDialogButtonBox(formLayoutWidget);
        launch_box->setObjectName(QStringLiteral("launch_box"));
        launch_box->setOrientation(Qt::Horizontal);
        launch_box->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        launch_box->setCenterButtons(false);

        form_layout->setWidget(8, QFormLayout::FieldRole, launch_box);

        SetupDialog->setCentralWidget(root);

        retranslateUi(SetupDialog);

        QMetaObject::connectSlotsByName(SetupDialog);
    } // setupUi

    void retranslateUi(QMainWindow *SetupDialog)
    {
        SetupDialog->setWindowTitle(QApplication::translate("SetupDialog", "Runtime setup", 0));
        label_fullscreen->setText(QApplication::translate("SetupDialog", "Full screen", 0));
        is_fullscreen->setText(QString());
        label_gpu->setText(QApplication::translate("SetupDialog", "GPU", 0));
        label_window_resolution->setText(QApplication::translate("SetupDialog", "Window resolution", 0));
        label_render_resolution->setText(QApplication::translate("SetupDialog", "Render resolution", 0));
        label_mouse_lock->setText(QApplication::translate("SetupDialog", "Mouse lock", 0));
        is_mouse_lock->setText(QString());
        label_scene->setText(QApplication::translate("SetupDialog", "Scene", 0));
        label_free_camera->setText(QApplication::translate("SetupDialog", "Free camera", 0));
        is_free_camera->setText(QString());
        label_frustum_culling->setText(QApplication::translate("SetupDialog", "Frustum culling", 0));
        is_frustum_cullling->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SetupDialog: public Ui_SetupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CONFIG_DESIGN_H
