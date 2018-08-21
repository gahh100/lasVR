#include "iconlibrary.h"
#include <QDir>
#include <QPainter>
#include <QtMath>
#include <QMessageBox>

GLuint IconLibrary::m_iconTex = 0;
QString IconLibrary::m_iconDir;
int IconLibrary::m_texWd = 0;
int IconLibrary::m_texHt = 0;
QImage IconLibrary::m_iconImage;
QStringList IconLibrary::m_iconList;
QList<QRect> IconLibrary::m_iconShape;
QList<QRectF> IconLibrary::m_iconGeom;

void
IconLibrary::init()
{
  m_iconTex = 0;
  m_iconDir.clear();
  m_iconList.clear();
  m_iconShape.clear();
  m_iconGeom.clear();
  m_texWd = m_texHt = 0;
}

void
IconLibrary::clear()
{
  if (m_iconTex)
    glDeleteTextures(1, &m_iconTex);
  
  m_iconTex = 0;
  m_iconDir.clear();
  m_iconList.clear();
  m_iconShape.clear();
  m_iconGeom.clear();
  m_texWd = m_texHt = 0;
}

QRect
IconLibrary::iconShape(QString iconName)
{
  for(int i=0; i<m_iconList.count(); i++)
    {
      if (m_iconList[i] == iconName)
	return m_iconShape[i];
    }

  return QRect();  
}

QRect
IconLibrary::iconShape(int i)
{
  if (i >=0 && i<m_iconList.count())
    return m_iconShape[i];

  return QRect();  
}

QRectF
IconLibrary::iconGeometry(QString iconName)
{
  for(int i=0; i<m_iconList.count(); i++)
    {
      if (m_iconList[i] == iconName)
	return m_iconGeom[i];
    }

  return QRectF();  
}

QRectF
IconLibrary::iconGeometry(int i)
{
  if (i >=0 && i<m_iconList.count())
    return m_iconGeom[i];

  return QRectF();  
}

void
IconLibrary::loadIcons(QString iconDir)
{
  m_iconDir = iconDir;
  m_iconList.clear();
  m_iconShape.clear();
  m_iconGeom.clear();

  QDir idir(m_iconDir);
  QStringList flist = idir.entryList(QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
  for(int i=0; i<flist.count(); i++)
    m_iconList << idir.relativeFilePath(flist.at(i));

  int iclsz = m_iconList.count();
  int iclsq = qSqrt(iclsz);
  m_texHt = m_texWd = iclsq*100;

  if (iclsq*iclsq < iclsz) iclsq++;
  m_texWd = iclsq*100;

  // texture size

  m_iconImage = QImage(m_texWd, m_texHt, QImage::Format_ARGB32);
  m_iconImage.fill(Qt::black);

  QPainter p(&m_iconImage);      
      
  for(int i=0; i<m_iconList.count(); i++)
    {
      int r = i/iclsq;
      int c = i%iclsq;
      m_iconShape << QRect(c*100, r*100, 100, 100);
      
      QImage iconImage = QImage(idir.absoluteFilePath(m_iconList[i])). \
	                 scaledToHeight(80, Qt::SmoothTransformation). \
	                 mirrored(false,true);
      
      p.drawImage(10 + c*100,
		  10 + r*100,
		  iconImage);

      m_iconGeom << QRectF(c*100.0/(float)m_texWd,
			   r*100.0/(float)m_texHt,
			   100.0/(float)m_texWd,
			   100.0/(float)m_texHt);
      
      QMessageBox::information(0, m_iconList[i],
			       QString("%1 %2 : %3 %4").\
			       arg(m_iconGeom[i].x()).\
			       arg(m_iconGeom[i].y()).\
			       arg(m_iconGeom[i].width()).\
			       arg(m_iconGeom[i].height()));
    }


  // load texture
  if (!m_iconTex)
    glGenTextures(1, &m_iconTex);
  
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, m_iconTex);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D,
	       0,
	       4,
	       m_texWd,
	       m_texHt,
	       0,
	       GL_RGBA,
	       GL_UNSIGNED_BYTE,
	       m_iconImage.bits());
  
  glDisable(GL_TEXTURE_2D);
}
