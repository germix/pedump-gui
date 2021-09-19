#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileDialog>
#include <QTreeView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QSettings>
#include <QCloseEvent>
#include <QMimeData>
#include "RecentFilesMenu.h"

#include "AboutDialog.h"
#include "pedump/pedump.h"

#define SETTINGS_APPLICATION "PEDumpGUI"
#define SETTINGS_ORGANIZATION "Germix"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);

	ui->setupUi(this);

	initRecentFilesMenu(s.value("RecentFiles").toByteArray());

	treeView = new QTreeView();
	treeView->setModel(model = new PEDumpModel());
	treeView->setHeaderHidden(true);
	treeView->setContextMenuPolicy(Qt::CustomContextMenu);
	treeView->installEventFilter(this);
	connect(treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotTreeView_doubleClicked(QModelIndex)));
	connect(treeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotTreeView_customContextMenuRequested(QPoint)));

	currentForm = NULL;
	contentWidget = new QWidget();
	QLayout* l = new QVBoxLayout();
	l->setContentsMargins(0, 0, 0, 0);
	contentWidget->setLayout(l);

	splitter = new QSplitter();
	splitter->addWidget(treeView);
	splitter->addWidget(contentWidget);
	splitter->setStretchFactor(0, 0);
	splitter->setStretchFactor(1, 1);

	setCentralWidget(splitter);

	ui->menu_View->addAction(ui->mainToolBar->toggleViewAction());

	//
	// Restaurar estado anterior
	//
	restoreGeometry(s.value("WindowGeometry").toByteArray());
	restoreState(s.value("WindowState").toByteArray());
	splitter->restoreGeometry(s.value("SplitterGeometry").toByteArray());
	splitter->restoreState(s.value("SplitterState").toByteArray());

	openFile(s.value("LastFile", "").toString());
}

MainWindow::~MainWindow()
{
	QSettings s(SETTINGS_ORGANIZATION, SETTINGS_APPLICATION);
	s.setValue("WindowState", saveState());
	s.setValue("WindowGeometry", saveGeometry());
	s.setValue("SplitterState", splitter->saveState());
	s.setValue("SplitterGeometry", splitter->saveGeometry());

	s.setValue("LastFile", currentFileName);
	s.setValue("RecentFiles", recentFiles->saveState());

	delete model;
	delete ui;
}

void MainWindow::dropEvent(QDropEvent* e)
{
	QString s;
	const QList<QUrl> urls = e->mimeData()->urls();

	// ...
	e->acceptProposedAction();
	for(int i = 0; i < urls.size(); i++)
	{
		s = urls.at(i).toLocalFile();
		if(!s.isEmpty())
		{
			openFile(s);
		}
	}
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
	if(e->mimeData()->hasFormat("text/uri-list"))
	{
		e->acceptProposedAction();
	}
}

void MainWindow::openFile()
{
	QString filters;

	filters += tr("All files") + " (*.*);;";

	QString fileName = QFileDialog::getOpenFileName(
				this,
				tr("Open file"),
				lastDirectory,
				filters);
	if(!fileName.isEmpty())
	{
		openFile(fileName);
	}
}

void MainWindow::openFile(const QString& fileName)
{
	if(fileName.isEmpty())
	{
		return;
	}
	closeFile();
	if(model->open(fileName))
	{
		recentFiles->removeFile(currentFileName);
		currentFileName = fileName;
		updateWindowTitle();
	}
}

bool MainWindow::closeFile()
{
	model->clear();
	recentFiles->addFile(currentFileName);
	currentFileName.clear();

	updateWindowTitle();
	setCurrentWidget(nullptr);

	return true;
}

void MainWindow::updateWindowTitle()
{
	QString s = "PEDumpGUI";

	if(!currentFileName.isEmpty())
	{
		s += " - " + QFileInfo(currentFileName).fileName();
	}

	setWindowTitle(s);
}

void MainWindow::initRecentFilesMenu(const QByteArray& state)
{
	recentFiles = new RecentFilesMenu(20, 10, tr("Recent files"), QIcon(":/images/clear.png"));

	ui->menu_File->insertMenu(ui->actionExit, recentFiles);
	ui->menu_File->insertSeparator(ui->actionExit);

	connect(recentFiles, SIGNAL(onFileTriggered(QString)), this, SLOT(slotRecentFiles_fileTriggered(QString)));

	recentFiles->restoreState(state);
}

void MainWindow::setCurrentWidget(QWidget* widget)
{
	QWidget* old = currentForm;
	if(currentForm != NULL)
	{
		currentForm->hide();
		contentWidget->layout()->removeWidget(currentForm);
	}
	currentForm = widget;
	if(widget != NULL)
	{
		contentWidget->layout()->addWidget(widget);
		currentForm->show();
	}
	if(old)
		delete old;
}

void MainWindow::slotAction()
{
	QAction* action = qobject_cast<QAction*>(sender());

	if(action == ui->actionExit)
	{
		close();
	}
	else if(action == ui->actionAbout)
	{
		AboutDialog().exec();
	}
	else if(action == ui->actionOpenFile)
	{
		openFile();
	}
}

void MainWindow::slotTreeView_doubleClicked(const QModelIndex& index)
{
	TreeItem* item;

	if(NULL != (item = model->toItem(index)))
	{
		setCurrentWidget(item->getWidget());
	}
}

void MainWindow::slotTreeView_customContextMenuRequested(const QPoint& pos)
{
}

void MainWindow::slotRecentFiles_fileTriggered(const QString& fileName)
{
	openFile(fileName);
}
