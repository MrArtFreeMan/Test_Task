#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestTask.h"

class TestTask : public QMainWindow
{
    Q_OBJECT

    public:
        TestTask(QWidget *parent = nullptr);
        ~TestTask();

    public slots:
        void OnAddTaskBtnClicked();
        void OnRemoveTaskBtnClicked();
        void OnSetItemDone(QListWidgetItem*);
    
    protected:
        void AddTaskToList(const QString&);
        void LoadTaskList(const QString&);
        void SaveTaskList(const QString&);
        void RestoreWindowSettings();
        void SaveWindowSettings();
    
    private:
        Ui::TestTaskClass ui;
};
