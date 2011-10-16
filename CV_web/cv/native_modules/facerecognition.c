#include <Python.h>
#include <opencv/cv.h>

#include <opencv2/highgui/highgui_c.h>

static PyObject *FaceError;

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

PyObject *detectFaces(IplImage *img)
{
    int i;

    /* Detect all faces on the frame */
    CvSeq *faces = cvHaarDetectObjects(
                                       img,
                                       cascade,
                                       storage,
                                       1.1,
                                       3, 0,
                                       cvSize(50, 50),
                                       cvSize(50, 50)
                                      );

    PyObject *result = PyList_New(0);

    /* Now, let's mark each face */
    for(i = 0 ; i < (faces ? faces->total : 0) ; i++) {
        CvRect *r = (CvRect*)cvGetSeqElem(faces, i);

        PyObject *origin = Py_BuildValue("{sisi}", "x", r->x, "y", r->y);
        PyObject *size = Py_BuildValue("{sisi}", "width", r->width, "height", r->height);
        PyObject *rect = Py_BuildValue("{sOsO}", "origin", origin, "size", size);

        PyList_Append(result, rect);
    }

    return result;
}


static PyObject *
face_detect(PyObject *self, PyObject *args)
{
    const char *path;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &path))
        return NULL;

    IplImage* frame;

    char *file = "/usr/local/share/opencv/"
        "/haarcascades/haarcascade_frontalface_alt.xml";

    cascade = (CvHaarClassifierCascade*)cvLoad(file, 0, 0, 0);
    storage = cvCreateMemStorage(0);

    IplImage* image = cvLoadImage(path, CV_LOAD_IMAGE_COLOR);

    if(image == NULL) {
        PyErr_SetString(FaceError, "Image not found!");
        return NULL;
    }

     PyObject *result = detectFaces(image);


     cvReleaseHaarClassifierCascade(&cascade);
     cvReleaseMemStorage(&storage);
     cvReleaseImage(&image);

     return result;
}

static PyMethodDef FaceMethods[] = {
    {"detect",  face_detect, METH_VARARGS,
     "Detect the faces on an image."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initface(void)
{
    PyObject *m;

    m = Py_InitModule("face", FaceMethods);
    if (m == NULL)
        return;

    FaceError = PyErr_NewException("face.error", NULL, NULL);
    Py_INCREF(FaceError);
    PyModule_AddObject(m, "error", FaceError);
}