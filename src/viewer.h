#ifndef VIEWER_H
#define VIEWER_H

#include "glewinitialisation.h"

#include "vr.h"

#include <QGLViewer/qglviewer.h>
using namespace qglviewer;

#include <QGLFramebufferObject>
#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>

#include "volumefactory.h"

#ifdef USE_GLMEDIA
#include "glmedia.h"
#endif // USE_GLMEDIA

//-------------------------------
// VR
//-------------------------------


class Viewer : public QGLViewer
{
  Q_OBJECT

 public :
  Viewer(QGLFormat&, QWidget *parent=0);
  ~Viewer();

  void setVolumeFactory(VolumeFactory *v) { m_volumeFactory = v; };
  VolumeFactory* volumeFactory() { return m_volumeFactory; };

  void setCoordMin(Vec cmin) { m_coordMin = cmin; };
  void setCoordMax(Vec cmax) { m_coordMax = cmax; };
  void setNumPoints(qint64 npt) { m_npoints = npt; };

  qint64 pointBudget() { return m_pointBudget; }

  void draw();
  void fastDraw();
  
  void wheelEvent(QWheelEvent*);

  void keyPressEvent(QKeyEvent*);
  
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void mouseDoubleClickEvent(QMouseEvent*);

  void start();

  void resizeGL(int, int);

  void paintGL();

  bool pointType() { return m_pointType; }
  void setPointType(bool p) { m_pointType = p; updateGL(); }

  int pointSize() { return m_pointSize; }
  void setPointSize(int p) { m_pointSize = p; updateGL(); }

  void setFlyMode(bool f) { m_flyMode = f; toggleCameraMode(); }

  void setSkybox(bool f) { m_skybox = f; m_vr.setSkyBox(m_skybox); }

  VR* vrPointer() { return &m_vr; }

  int currentTime() { return m_currTime; }

  void setNearFar(float n, float f) { m_nearDist = n; m_farDist = f; }

  Vec menuCamPos();

  bool editMode() { return m_editMode; }

  void setEditMode(bool);
  void toggleEditMode();

  void setCamMode(bool);
  void toggleCamMode();

  void centerPointClouds();
  void alignUsingPointPairs();

  void undo();
  void saveModInfo();

  void setImageMode(int);
  void setImageFileName(QString);
  void setSaveSnapshots(bool);
  void setSaveMovie(bool);
  void setImageSize(int, int);

  bool startMovie(QString, int, int, bool);

  int maxTimeStep() { return m_maxTime; }
  int currentTimeStep() { return m_currTime; }

  void setEdges(bool b) { m_showEdges = b; update(); }
  void setSoftShadows(bool b) { m_softShadows = b; update(); }
  void setSpheres(bool b) { m_showSphere = b; update(); }

  QList<PointCloud*> pointCloudList() { return m_pointClouds; }

  public slots :
    void GlewInit();
    void showBox(bool);

    void vboLoaded(int, qint64);
    void vboLoadedAll(int, qint64);

    void meshLoadedAll();
    
    void loadLinkOnTop(QString);
    void loadLink(QString);
    void loadLink(QStringList);

    void updateFramerate();
    void setPointBudget(int);

    bool setVRMode(bool);
    bool vrMode() { return m_vrMode; }

    void setKeyFrame(int);
    void updateLookFrom(Vec, Quaternion, int);

    void setCurrentFrame(int);
    void endPlay();

    void captureKeyFrameImage(int);

    void setTimeStep(int);

    void clearLoadNodeList() { m_loadNodes.clear(); }

    void addTempLabel(Vec, QString);
    void moveTempLabel(Vec);
    void makeTempLabelPermanent();
    void addLabel(Vec, QString);
    void loadLabelsJson(QString);
    
 signals :
    void showToolbar();
    void nextFrame();
    void nextKeyFrame();
    void loadLinkedData(QString);
    void loadLinkedData(QStringList);
    void switchVolume();
    void bbupdated(Vec, Vec);
    void loadPointsToVBO();
    void setVBOs(GLuint, GLuint);
    void setVisTex(GLuint);
    void stopLoading();
    void framesPerSecond(float);
    void message(QString);
    void updateView();
    void removeEditedNodes();
    void setKeyFrame(Vec, Quaternion, int, QImage, int);
    void replaceKeyFrameImage(int, QImage);
    void timeStepChanged(int);

 private :
    VR m_vr;

    Volume* m_volume;
    VolumeFactory* m_volumeFactory;

    Vec m_coordMin, m_coordMax;
    qint64 m_npoints;

    int m_firstImageDone;
    bool m_moveViewerToCenter;
    bool m_vboLoadedAll;

    QList<PointCloud*> m_pointClouds;
    QList<OctreeNode*> m_tiles;
    QList<OctreeNode*> m_orderedTiles;
    QList<OctreeNode*> m_loadNodes;
    QMultiMap<float, OctreeNode*> m_priorityQueue;


    int m_numTrisetVBOs;
    GLuint *m_trisetVBOs;
    bool m_meshLoadedAll;    
    QList<Triset*> m_trisets;


    GLhandleARB m_shadowShader;
    GLint m_shadowParm[10];

    GLhandleARB m_smoothShader;
    GLint m_smoothParm[10];
  
    bool m_fastDraw;

    bool m_pointType;
    bool m_skybox;
    bool m_flyMode;
    bool m_showInfo;
    bool m_showPoints;
    bool m_showBox;
    int m_pointSize;
    int m_dpv;

    int m_currTime;
    int m_maxTime;
    bool m_currTimeChanged;

    GLuint m_vertexArrayID;
    int m_vbID;
    qint64 m_vbPoints;
    GLuint m_vertexBuffer[2];

    GLuint m_vertexScreenBuffer;
    float m_scrGeo[8];

    GLuint m_depthBuffer;
    GLuint m_rbo;
    GLuint m_depthTex[4];

    GLuint m_colorTex;
    uchar *m_colorMap;
    QList<Vec> m_colorGrad;

    GLhandleARB m_depthShader;
    GLint m_depthParm[50];

    GLhandleARB m_blurShader;
    GLint m_blurParm[20];

    GLhandleARB m_meshShader;
    GLint m_meshParm[20];

    bool m_showEdges;
    bool m_softShadows;
    bool m_smoothDepth;
    bool m_showSphere;

    bool m_selectActive;
    qint64 m_pointBudget;
    qint64 m_pointsDrawn;

    int m_minNodePixelSize;

    int m_headSetType;

    Vec m_fruCnr[8];
    GLdouble m_planeCoefficients[6][4];

    QTime m_flyTime;

    QFile m_pFile;
    QString pickPointsFiles;
    bool m_savePointsToFile;

    GLuint m_minmaxTex;
    float *m_minmaxMap;

    GLuint m_visibilityTex;
    uchar *m_visibilityMap;

    int m_origWidth;
    int m_origHeight;

    float m_nearDist, m_farDist;

    QPoint m_backButtonPos;

    QVector3D m_hmdPos, m_hmdVD, m_hmdUP, m_hmdRV;
    Vec m_lp;

    int m_frames;

    Camera m_menuCam;

    QString m_dataDir;

    bool m_vrModeSwitched;
    bool m_vrMode;
    bool m_editMode;
    QPoint m_prevPos;
    int m_moveAxis;
    Vec m_deltaShift;
    float m_deltaScale;
    Quaternion m_deltaRot;
    bool m_deltaChanged;
    QList<QVector4D> m_undoXform;

    int m_imageWidth, m_imageHeight;
    int m_currFrame;
    int m_imageMode;
    bool m_saveSnapshots;
    bool m_saveMovie;
    QString m_imageFileName;

#ifdef USE_GLMEDIA
    glmedia_movie_writer_t m_movieWriter;
#endif // USE_GLMEDIA
    unsigned char *m_movieFrame;

    
    QList<Vec> m_pointPairs;


    void createFBO();

    void genColorMap();

    void generateVBOs();

    void drawPointsWithReload();
    void drawPoints(vr::Hmd_Eye);
    void drawPointsWithShadows(vr::Hmd_Eye);
    
    void reset();

    void drawAABB();

    void drawInfo();

    void createShaders();

    void drawGeometry();

    Vec closestPointOnBox(Vec, Vec, Vec);

    void loadNodeData();

    void drawVAO();

    bool isVisible(Vec, Vec);
    bool isVisible(Vec, Vec,
		   int, int, int, int);

    void genDrawNodeList();
    bool genDrawNodeList(float, float);
    void orderTilesForCamera();


    void genDrawNodeListForVR();


    void calcFrustrumCornerPointsAndPlaneCoeff(float);

    bool nearCam(Vec, Vec, Vec, Vec, float);

    void savePointsToFile(Vec);

    void drawLabels();
    void drawLabelsForVR(vr::Hmd_Eye);

    void createMinMaxTexture();
    void createVisibilityTexture();

    void dummydraw();

    bool linkClicked(QMouseEvent*);

    void generateFirstImage();

    void loadTopJson(QString);
    void saveTopJson(QString);

    void stickLabelsToGround();

    void rotatePointCloud(QMouseEvent*, QPoint);
    void movePointCloud(QPoint);
    void scalePointCloud(QPoint);

    void saveImage();
    void saveMonoImage(QString, QChar, int);
    void saveSnapshot(QString);

    void saveMovie();
    bool endMovie();

    void drawPointPairs();

    void genTrisetsList();
    void drawTrisets();
    void drawTrisets(vr::Hmd_Eye);
};

#endif
