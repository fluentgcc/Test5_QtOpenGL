#pragma once

#include <QOpenGLWindow>


class RenderWindow : public QOpenGLWindow
{
	Q_OBJECT
public:
	RenderWindow(QWindow *parent = nullptr);
	~RenderWindow();

protected:
	void initializeGL() override;	
	void resizeGL(int w, int h) override;
	void paintGL() override;

//	void keyPressEvent(QKeyEvent *e) override;
//	void mouseMoveEvent(QMouseEvent *) override;
//	void mousePressEvent(QMouseEvent *) override;
//	void mouseReleaseEvent(QMouseEvent *) override;


private:


};
