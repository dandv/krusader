/***************************************************************************
                      _expressionsu.cpp  -  description
                         -------------------
begin                : Sat Dec 6 2003
copyright            : (C) 2003 by Shie Erlich & Rafi Yanai
email                :
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "usermenu.h"
#include "../krusaderview.h"
#include "../krusader.h"
#include "../defaults.h"
#include "../Panel/listpanel.h"
#include <kdebug.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <qvbox.h>

/**

Command: %xYYY%
         x - can be either 'a' for active panel, or 'o' for other panel
         YYY - the specific command

         For example:
           %ap% - active panel path
           %op% - other panel path

In the following commands, we'll use '_' instead of 'a'/'o'. Please substitute as needed.

%_p%    - panel path
%_c%    - current file (or folder). Note: current != selected
%_s%    - selected files and folders
%_cs%   - selected files including current file (if it's not already selected)
%_afd%  - all files and folders
%_af%   - all files (not including folders)
%_ad%   - all folders (not including files)
%_an%   - number of files and folders
%_anf%  - number of files
%_and%  - number of folders
%_fm%   - filter mask (for example: *, *.cpp, *.h etc.)

*/
UMCmd UserMenu::_expressions[ NUM_EXPS ] = {
         {"%_p%", expPath}
      };

#define ACTIVE  krApp->mainView->activePanel
#define OTHER   krApp->mainView->activePanel->otherPanel

QString UserMenu::expPath( const QString& str ) {
   if ( str.lower() == "%ap%" ) {
      return ACTIVE->virtualPath;
   } else if ( str.lower() == "%op%" ) {
      return OTHER->virtualPath;
   } else return QString::null;
}

void UserMenu::exec() {
   // execute menu and wait for selection
   QString cmd = _popup.run();

   // replace %% and prepare string
   cmd = expand( cmd );
   //kdWarning() << cmd << endl;

   // ............... run the cmd from the shell .............
   //QString save = getcwd( 0, 0 );
   //===> chdir( panelPath.local8Bit() ); // run the command in the panel's path

   // run in  terminal
   //proc.setUseShell(true);
   //proc << "konsole" << "--noclose" << "-e" << cmd;
   //proc.start( KProcess::DontCare );

   UserMenuProc *proc = new UserMenuProc( UserMenuProc::Terminal, true );
   proc->start( cmd );
   //===> chdir( save.local8Bit() ); // chdir back
}

QString UserMenu::expand( QString str ) {
   QString result = QString::null, exp = QString::null;
   int beg, end, i;
   unsigned int idx = 0;
   while ( idx < str.length() ) {
      if ( ( beg = str.find( '%', idx ) ) == -1 ) break;
      if ( ( end = str.find( '%', beg + 1 ) ) == -1 ) {
         kdWarning() << "Error: unterminated % in UserMenu::expand" << endl;
         return QString::null;
      }
      result += str.mid( idx, beg - idx ); // copy until the start of %exp%

      // get the expression, and expand it using the correct expander function
      // ... replace first char with _ to ease the checking
      exp = str.mid( beg, end - beg + 1 );
      for ( i = 0; i < NUM_EXPS; ++i )
         if ( str.mid( beg, end - beg + 1 ).replace( 1, 1, '_' ) == _expressions[ i ].expression ) {
            result += ( _expressions[ i ].expFunc ) ( exp );
            break;
         }
      if ( i == NUM_EXPS ) { // didn't find an expander
         kdWarning() << "Error: unrecognized " << exp << " in UserMenu::expand" << endl;
         return QString::null;
      }
      idx = end + 1;
   }
   // copy the rest of the string
   result += str.mid( idx );

   return result;
}

UserMenu::UserMenu( QWidget * parent, const char * name ) : QWidget( parent, name ) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

UserMenuGui::UserMenuGui( QWidget * parent ) : KPopupMenu( parent ) {
   insertTitle( "User Menu" );

   // read entries from config file.
   readEntries();


   // fill popup menu. Note: this code assumes that the _entries list contains pairs
   // of {descripion, command}. If this is not the case, the code will fail.
   // However, it should have been checked above (read entries from file)
   int idx = 1;
   for ( QStringList::Iterator it = _entries.begin(); it != _entries.end(); ++it ) {
      insertItem( *it, idx++ );
      ++it;
   }

   // add the "add new entry" command
   insertSeparator();
   insertItem( "Add new entry", 0 );
}

void UserMenuGui::readEntries() {
   // Note: entries are marked 1..n, so that entry 0 is always
   // available. It is used by the "add new entry" command.
   QString filename = locateLocal( "data", "krusader/krusermenu.dat" );
   int i = 0;

   QFile file( filename );
   if ( file.open( IO_ReadOnly ) ) {
      QTextStream stream( &file );
      QString line;

      while ( !stream.atEnd() ) {
         line = stream.readLine();
         _entries += line;
         ++i;
      }
      file.close();
   }
   // do we need to remove last entry?
   if ( i > 0 && i % 2 != 0 ) _entries.pop_back();

}

QString UserMenuGui::run() {
   int idx = exec();
   if ( idx == -1 ) return QString::null; // nothing was selected
   if ( idx == 0 ) return QString::null; // todo: insert gui here

   // idx is {1..n} while _entries is {0..n-1}X2
   // so, normalize idx to _entries, and add 1 since we want
   // the command part of the {description,command} pair
   return _entries[ ( idx -1 ) * 2 + 1 ];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

UserMenuProcDlg::UserMenuProcDlg( QString caption, bool enableStderr, QWidget *parent ) :
KDialogBase( parent, 0, false, caption, KDialogBase::Ok | KDialogBase::Cancel, KDialogBase::Cancel ) {

   setButtonOKText( "Close", i18n( "Close this window" ) );
   enableButtonOK( false ); // disable the close button, until the process finishes

   setButtonCancelText( "Kill", i18n( "Kill the running process" ) );

   QVBox *page = makeVBoxMainWidget();
   // do we need to separate stderr and stdout?
   if ( enableStderr ) {
      // create stdout
      new QLabel( i18n( "Standard Output (stdout)" ), page );
      _stdout = new QTextEdit( page );
      _stdout->setReadOnly( true );
      _stdout->setMinimumWidth( fontMetrics().maxWidth() * 40 );
      // create stderr
      new QLabel( i18n( "Standard Error (stderr)" ), page );
      _stderr = new QTextEdit( page );
      _stderr->setReadOnly( true );
      _stderr->setMinimumWidth( fontMetrics().maxWidth() * 40 );
   } else {
      // create stdout
      new QLabel( i18n( "Output" ), page );
      _stdout = new QTextEdit( page );
      _stdout->setReadOnly( true );
      _stdout->setMinimumWidth( fontMetrics().maxWidth() * 40 );
   }
}

void UserMenuProcDlg::addStderr( KProcess *proc, char *buffer, int buflen ) {
   _stderr->append( QString::fromLatin1( buffer, buflen ) );
}

void UserMenuProcDlg::addStdout( KProcess *proc, char *buffer, int buflen ) {
   _stdout->append( QString::fromLatin1( buffer, buflen ) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


UserMenuProc::UserMenuProc( ExecType execType, bool enableStderr ) : QObject(), _execType( execType ),
_enableStderr( enableStderr ), _proc( new KProcess() ), _output( 0 ) {
   _proc->setUseShell( true );

   connect( _proc, SIGNAL( processExited( KProcess* ) ),
            this, SLOT( processExited( KProcess* ) ) ) ;
}


UserMenuProc::~UserMenuProc() {
   delete _proc;
}

bool UserMenuProc::start( QString cmdLine ) {
   _proc->clearArguments();

   // run in terminal or in custom-window
   if ( _execType == Terminal ) {
      ( *_proc ) << "konsole" << "--noclose" << "-e" << cmdLine;
   } else if ( _execType == OutputOnly ) { // output only - collect output
      ( *_proc ) << cmdLine;
      _output = new UserMenuProcDlg( cmdLine, _enableStderr );
      // connect the output to the dialog
      connect( _proc, SIGNAL( receivedStderr( KProcess*, char*, int ) ),
               _output, SLOT( addStderr( KProcess*, char *, int ) ) );
      connect( _proc, SIGNAL( receivedStdout( KProcess*, char*, int ) ),
               _output, SLOT( addStdout( KProcess*, char *, int ) ) );
      connect( _output, SIGNAL( cancelClicked() ), this, SLOT( kill() ) );
      _output->show();
   } else { // no terminal, no output collection
      ( *_proc ) << cmdLine;
   }
   return _proc->start( KProcess::NotifyOnExit, ( KProcess::Communication ) ( KProcess::Stdout | KProcess::Stderr ) );
}

void UserMenuProc::processExited( KProcess *proc ) {
   // enable the 'close' button on the dialog (if active), disable 'kill' button
   if ( _output ) {
      _output->enableButtonOK( true );
      _output->enableButtonCancel( false);
   }
   delete this; // banzai!!
}
