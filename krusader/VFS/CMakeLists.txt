
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES}  )


########### next target ###############


########### install files ###############




#original Makefile.am contents follow:

#noinst_LIBRARIES = libVFS.a
#
#INCLUDES = $(all_includes)
#
#libVFS_a_METASOURCES = AUTO
#
#libVFS_a_SOURCES = \
#	krvfshandler.cpp \
#	virt_vfs.cpp \
#	vfs.cpp \
#	vfile.cpp \
#	temp_vfs.cpp \
#	normal_vfs.cpp \
#	krpermhandler.cpp \
#	krdirwatch.cpp \
#	krarchandler.cpp \
#	preservingcopyjob.cpp \
#	virtualcopyjob.cpp \
#	ftp_vfs.cpp krquery.cpp