#include "TestTask.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QSettings>

static QString TASK_LIST_FILE_NAME = "TaskList.txt";

TestTask::TestTask(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.addTaskBtn, &QPushButton::clicked, this, &TestTask::OnAddTaskBtnClicked);
    connect(ui.removeTaskBtn, &QPushButton::clicked, this, &TestTask::OnRemoveTaskBtnClicked);
    connect(ui.taskList, &QListWidget::itemChanged, this, & TestTask::OnSetItemDone);

    RestoreWindowSettings();
    LoadTaskList(TASK_LIST_FILE_NAME);
}

TestTask::~TestTask()
{
    SaveTaskList(TASK_LIST_FILE_NAME);
    SaveWindowSettings();
}
//---------------------------------------------------------------------------------------------------------
void TestTask::AddTaskToList(const QString& taskName)
{
    if (taskName.isEmpty())
    {
        return;
    }
    QListWidgetItem* item = new QListWidgetItem(taskName);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    ui.taskList->addItem(item);
}

void TestTask::LoadTaskList(const QString& name) 
{
    QFile listFile(name);
    if (listFile.exists())
    {
        if (listFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&listFile);
            while (!in.atEnd())
            {
                QString taskName = in.readLine();
                AddTaskToList(taskName);
            }
        }
    }
    listFile.close();
}

void TestTask::SaveTaskList(const QString& name) 
{
    QFile listFile(name);
    if (listFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&listFile);
        for (int i = 0; i < ui.taskList->count(); ++i)
        {
            out << ui.taskList->item(i)->text() << "\n";
        }
    }
    listFile.close();
}
//---------------------------------------------------------------------------------------------------------
void TestTask::RestoreWindowSettings() 
{
    const QSize defaultWindowSize = QSize(400,400);
    QSettings restore("AnyOrg", "TestTask");
    QSize windowSize = restore.value("window_size", defaultWindowSize).toSize();
    this->resize(windowSize);
}

void TestTask::SaveWindowSettings() 
{
    QSettings save("AnyOrg", "TestTask");
    save.setValue("window_size", this->size());
}
//---------------------------------------------------------------------------------------------------------
void TestTask::OnAddTaskBtnClicked() 
{
    QString taskName = ui.taskNameLE->text();
    if (taskName.isEmpty())
    {
        QMessageBox::critical(this, "TestTask:", "Error: task with empty name can not be added...");
        return;
    }
    AddTaskToList(taskName);
}

void TestTask::OnRemoveTaskBtnClicked() 
{
    QList<QListWidgetItem*> selectedItems = ui.taskList->selectedItems();
    for (auto item : selectedItems) 
    {
        delete ui.taskList->takeItem(ui.taskList->row(item));
    }
}

void TestTask::OnSetItemDone(QListWidgetItem* item)
{
    bool isDone = item->checkState() == Qt::Checked; 
    {
        QFont itemFont = item->font();
        itemFont.setStrikeOut(isDone);
        itemFont.setBold(isDone);
        item->setFont(itemFont);
    }
}
