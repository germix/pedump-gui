#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QTreeView;
class QSplitter;
class RecentFilesMenu;

class PEDumpModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	Ui::MainWindow*		ui;

	QString				lastDirectory;

	QTreeView*			treeView;
	QSplitter*			splitter;
	QWidget*			currentForm;
	QWidget*			contentWidget;

	PEDumpModel*		model;

	RecentFilesMenu*	recentFiles;

	QString				currentFileName;
public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();
private:
	void dropEvent(QDropEvent* e);
	void dragEnterEvent(QDragEnterEvent* e);

private:
	void openFile();
	void openFile(const QString& fileName);
	bool closeFile();
	void updateWindowTitle();
	void initRecentFilesMenu(const QByteArray& state);
	void setCurrentWidget(QWidget* widget);
private slots:
	void slotAction();

	void slotTreeView_doubleClicked(const QModelIndex& index);
	void slotTreeView_customContextMenuRequested(const QPoint& pos);

	void slotRecentFiles_fileTriggered(const QString& fileName);
};

#endif // MAINWINDOW_H
