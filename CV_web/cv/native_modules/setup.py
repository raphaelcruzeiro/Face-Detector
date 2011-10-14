from distutils.core import setup, Extension

module1 = Extension('face',
                    include_dirs = ['/usr/local/include'],
                    libraries = ['opencv_core', 'opencv_objdetect'],
                    library_dirs = ['/usr/local/lib'],
                    sources = ['facerecognition.c']
                    )

setup (name = 'Face CV Web',
       version = '1.0',
       description = 'A computer vision module.',
       ext_modules = [module1])