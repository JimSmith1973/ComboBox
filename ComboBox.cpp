// ComboBox.cpp

#include "ComboBox.h"

// Global variables
ButtonWindow g_buttonWindow;
ComboBoxWindow g_comboBoxWindow;
ListBoxWindow g_listBoxWindow;
StatusBarWindow g_statusBarWindow;

void ComboBoxWindowSelectionChangedFunction( LPTSTR lpszItemText )
{
	FileFind fileFind;

	// Find first item in folder
	if( fileFind.First( lpszItemText, ALL_FILES_FILTER ) )
	{
		// Successfully found first item in folder
		int nFileCount = 0;

		// Allocate string memory
		LPTSTR lpszFoundItemName = new char[ STRING_LENGTH + sizeof( char ) ];
		LPTSTR lpszStatusMessage = new char[ STRING_LENGTH + sizeof( char ) ];

		// Delete all items from list box window
		g_listBoxWindow.ResetContent();

		// Loop through all items
		do
		{
			// See if found item is a file
			if( fileFind.IsFile() )
			{
				// Found item is a file

				// Get found item name
				fileFind.GetFileName( lpszFoundItemName );

				// Add found item to list box window
				g_listBoxWindow.AddText( lpszFoundItemName );

				// Update file count
				nFileCount ++;

			} // End of found item is a file

		} while( fileFind.Next() ); // End of loop through all items

		// Format status message
		wsprintf( lpszStatusMessage, FILE_FIND_CLASS_FOUND_FILES_STATUS_MESSAGE_FORMAT_STRING, lpszItemText, nFileCount );

		// Show status message on status bar window
		g_statusBarWindow.SetText( lpszStatusMessage );

		// Close file find
		fileFind.Close();

		// Free string memory
		delete [] lpszFoundItemName;
		delete [] lpszStatusMessage;

	} // End of successfully found first item in folder

} // End of function ListBoxWindowSelectionChangedFunction

void ListBoxWindowSelectionChangedFunction( LPTSTR lpszItemText )
{
	// Show item text on status bar window
	g_statusBarWindow.SetText( lpszItemText );

} // End of function ListBoxWindowSelectionChangedFunction

void ListBoxWindowDoubleClickFunction( LPTSTR lpszItemText )
{
	// Display item text
	MessageBox( NULL, lpszItemText, INFORMATION_MESSAGE_CAPTION, ( MB_OK | MB_ICONINFORMATION ) );

} // End of function ListBoxWindowDoubleClickFunction

void OpenFileFunction( LPCTSTR lpszFilePath )
{
	// Add file to list box window
	g_listBoxWindow.AddText( lpszFilePath );

} // End of function OpenFileFunction

int ShowAboutMessage( HWND hWndParent )
{
	int nResult = 0;

	MSGBOXPARAMS mbp;

	// Clear message box parameter structure
	ZeroMemory( &mbp, sizeof( mbp ) );

	// Initialise message box parameter structure
	mbp.cbSize		= sizeof( MSGBOXPARAMS );
	mbp.hwndOwner	= hWndParent;
	mbp.hInstance	= NULL;
	mbp.lpszText	= ABOUT_MESSAGE_TEXT;
	mbp.lpszCaption	= ABOUT_MESSAGE_CAPTION;
	mbp.dwStyle		= ( MB_OK | MB_USERICON );
	mbp.lpszIcon	= WINDOW_CLASS_CLASS_DEFAULT_ICON_NAME;

	// Show message box
	nResult = MessageBoxIndirect( &mbp );

	return nResult;

} // End of function ShowAboutMessage

LRESULT CALLBACK MainWindowProcedure( HWND hWndMain, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	LRESULT lr = 0;

	// Select message
	switch( uMessage )
	{
		case WM_CREATE:
		{
			// A create message
			HINSTANCE hInstance;

			// Get instance
			hInstance = ( ( LPCREATESTRUCT )lParam )->hInstance;

			// Create combo box window
			if( g_comboBoxWindow.Create( hWndMain, hInstance ) )
			{
				// Successfully created combo box window
				Font font;

				// Get font
				font = DEFAULT_GUI_FONT;

				// Set combo box window font
				g_comboBoxWindow.SetFont( font );

				// Create button window
				if( g_buttonWindow.Create( hWndMain, hInstance, BUTTON_WINDOW_TEXT, BUTTON_WINDOW_ID ) )
				{
					// Successfully created button window

					// Set button window font
					g_buttonWindow.SetFont( font );

					// Create list box window
					if( g_listBoxWindow.Create( hWndMain, hInstance ) )
					{
						// Successfully created list box window

						// Set list box window font
						g_listBoxWindow.SetFont( font );

						// Create status bar window
						if( g_statusBarWindow.Create( hWndMain, hInstance ) )
						{
							// Successfully created status bar window

							// Set status bar window font
							g_statusBarWindow.SetFont( font );

						} // End of successfully created status bar window

					} // End of successfully created list box window

				} // End of successfully created button window

			} // End of successfully created combo box window

			// Break out of switch
			break;

		} // End of a create message
		case WM_SIZE:
		{
			// A size message
			int nClientWidth;
			int nClientHeight;
			RECT rcStatus;
			RECT rcComboBox;
			int nStatusWindowHeight;
			int nComboBoxWindowHeight;
			int nListBoxWindowHeight;
			int nListBoxWindowTop;
			int nComboBoxWindowWidth;
			int nButtonWindowLeft;

			// Store client width and height
			nClientWidth	= ( int )LOWORD( lParam );
			nClientHeight	= ( int )HIWORD( lParam );

			// Size status bar window
			g_statusBarWindow.Size();

			// Get window sizes
			g_statusBarWindow.GetWindowRect( &rcStatus );
			g_comboBoxWindow.GetWindowRect( &rcComboBox );

			// Calculate window sizes
			nStatusWindowHeight		= ( rcStatus.bottom - rcStatus.top );
			nComboBoxWindowHeight	= ( rcComboBox.bottom - rcComboBox.top );
			nListBoxWindowHeight	= ( nClientHeight - ( nStatusWindowHeight + nComboBoxWindowHeight ) + WINDOW_BORDER_HEIGHT );
			nComboBoxWindowWidth	= ( ( nClientWidth - BUTTON_WINDOW_WIDTH ) + WINDOW_BORDER_WIDTH );

			// Calculate window positions
			nListBoxWindowTop		= ( nComboBoxWindowHeight - WINDOW_BORDER_HEIGHT );
			nButtonWindowLeft		= ( nComboBoxWindowWidth - WINDOW_BORDER_WIDTH );

			// Move control windows
			g_comboBoxWindow.Move( 0, 0, nComboBoxWindowWidth, nClientHeight, TRUE );
			g_buttonWindow.Move( nButtonWindowLeft, 0, BUTTON_WINDOW_WIDTH, nComboBoxWindowHeight );
			g_listBoxWindow.Move( 0, nListBoxWindowTop, nClientWidth, nListBoxWindowHeight, TRUE );

			// Break out of switch
			break;

		} // End of a size message
		case WM_ACTIVATE:
		{
			// An activate message

			// Focus on list box window
			g_listBoxWindow.SetFocus();

			// Break out of switch
			break;

		} // End of an activate message
		case WM_GETMINMAXINFO:
		{
			// A get min max info message
			MINMAXINFO FAR *lpMinMaxInfo;

			// Get min max info structure
			lpMinMaxInfo = ( MINMAXINFO FAR * )lParam;

			// Update min max info structure
			lpMinMaxInfo->ptMinTrackSize.x = WINDOW_CLASS_DEFAULT_MINIMUM_WIDTH;
			lpMinMaxInfo->ptMinTrackSize.y = WINDOW_CLASS_DEFAULT_MINIMUM_HEIGHT;

			// Break out of switch
			break;

		} // End of a get min max info message
		case WM_DROPFILES:
		{
			// A drop files message
			DroppedFiles droppedFiles;

			// Get dropped files
			if( droppedFiles.Get( wParam ) )
			{
				// Successfully got dropped files

				// Process dropped files
				droppedFiles.Process( &OpenFileFunction );

			} // End of successfully got dropped files

			// Break out of switch
			break;

		} // End of a drop files message
		case WM_COMMAND:
		{
			// A command message

			// Select command
			switch( LOWORD( wParam ) )
			{
				case BUTTON_WINDOW_ID:
				{
					// A button window command
					BrowseForFolder bff;

					// Allocate string memory
					LPTSTR lpszFolderPath = new char[ STRING_LENGTH + sizeof( char ) ];

					// Initialise file path
					GetCurrentDirectory( STRING_LENGTH, lpszFolderPath );

					// Browse for folder
					if( bff.Browse( lpszFolderPath ) )
					{
						// Successfully browsed for folder
						int nWhichItem;

						// Add folder to combo box window
						nWhichItem = g_comboBoxWindow.AddUniqueItem( lpszFolderPath );

						// Ensure that folder was added to combo box window
						if( nWhichItem >= 0 )
						{
							// Successfully added folder to combo box window

							// Select folder on combo box window
							g_comboBoxWindow.SelectItem( nWhichItem );

							// Call selection changed function
							ComboBoxWindowSelectionChangedFunction( lpszFolderPath );

						} // End of successfully added folder to combo box window

					} // End of successfully browsed for folder

					// Free string memory
					delete [] lpszFolderPath;

					// Break out of switch
					break;

				} // End of a button window command
				case IDM_FILE_EXIT:
				{
					// A file exit command

					// Destroy window
					DestroyWindow( hWndMain );

					// Break out of switch
					break;

				} // End of a file exit command
				case IDM_HELP_ABOUT:
				{
					// A help about command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about command
				default:
				{
					// Default command

					// See if command message is from combo box window
					if( ( HWND )lParam == g_comboBoxWindow )
					{
						// Command message is from combo box window

						// Handle command message from combo box window
						if( !( g_comboBoxWindow.HandleCommandMessage( wParam, lParam, ComboBoxWindowSelectionChangedFunction ) ) )
						{
							// Command message was not handled from combo box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command message was not handled from combo box window

					} // End of command message is from combo box window
					else if( ( HWND )lParam == g_listBoxWindow )
					{
						// Command message is from list box window

						// Handle command message from list box window
						if( !( g_listBoxWindow.HandleCommandMessage( wParam, lParam, ListBoxWindowSelectionChangedFunction, ListBoxWindowDoubleClickFunction ) ) )
						{
							// Command message was not handled from list box window

							// Call default procedure
							lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

						} // End of command message was not handled from list box window

					} // End of command message is from list box window
					else
					{
						// Command message is not from list box window

						// Call default procedure
						lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					} // End of command message is not from list box window

					// Break out of switch
					break;

				} // End of default command

			}; // End of selection for command

			// Break out of switch
			break;

		} // End of a command message
		case WM_SYSCOMMAND:
		{
			// A system command message

			// Select system command
			switch( LOWORD( wParam ) )
			{
				case IDM_HELP_ABOUT:
				{
					// A help about system command

					// Show about message
					ShowAboutMessage( hWndMain );

					// Break out of switch
					break;

				} // End of a help about system command
				default:
				{
					// Default system command

					// Call default procedure
					lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

					// Break out of switch
					break;

				} // End of default system command

			}; // End of selection for system command

			// Break out of switch
			break;

		} // End of a system command message
		case WM_NOTIFY:
		{
			// A notify message

			// Call default handler
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of a notify message
		case WM_CONTEXTMENU:
		{
			// A context menu message
			Menu popupMenu;

			// Load popup menu
			popupMenu.Load( MAKEINTRESOURCE( IDR_CONTEXT_MENU ) );

			// Show popup menu
			popupMenu.TrackPopupMenu( 0, lParam, hWndMain );

			// Break out of switch
			break;

		} // End of a context menu message
		case WM_CLOSE:
		{
			// A close message

			// Save combo box window
			if( g_comboBoxWindow.Save( COMBO_BOX_FILE_NAME ) )
			{
				// Successfully saved combo box window

				// Destroy main window
				DestroyWindow( hWndMain );

			} // End of successfully saved combo box window
			else
			{
				// Unable to save combo box window

				// Ensure that it is ok to close
				if( MessageBox( hWndMain, COMBO_BOX_WINDOW_CLASS_UNABLE_TO_SAVE_WARNING_MESSAGE, WARNING_MESSAGE_CAPTION, ( MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2 ) ) == IDYES )
				{
					// Is ok to close

					// Destroy main window
					DestroyWindow( hWndMain );

				} // End of is ok to close

			} // End of unable to save combo box window

			// Break out of switch
			break;

		} // End of a close message
		case WM_DESTROY:
		{
			// A destroy message

			// Terminate thread
			PostQuitMessage( 0 );

			// Break out of switch
			break;

		} // End of a destroy message
		default:
		{
			// Default message

			// Call default window procedure
			lr = DefWindowProc( hWndMain, uMessage, wParam, lParam );

			// Break out of switch
			break;

		} // End of default message

	}; // End of selection for message

	return lr;

} // End of function MainWindowProcedure

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow )
{
	Message message;

	WindowClass mainWindowClass;

	// Initialise main window class
	mainWindowClass.Initialise( MAIN_WINDOW_CLASS_NAME, hInstance, MainWindowProcedure, MAIN_WINDOW_CLASS_MENU_NAME );

	// Register main window class
	if( mainWindowClass.Register() )
	{
		// Successfully registered main window class
		Window mainWindow;

		// Create main window
		if( mainWindow.Create( MAIN_WINDOW_CLASS_NAME, NULL, hInstance, MAIN_WINDOW_TEXT ) )
		{
			// Successfully created main window
			Menu systemMenu;
			ArgumentList argumentList;

			// Get system menu
			systemMenu.GetSystem( mainWindow );

			// Add separator item to system menu
			systemMenu.InsertSeparator( MENU_CLASS_SYSTEM_MENU_SEPARATOR_ITEM_POSITION );

			// Add about item to system menu
			systemMenu.InsertItem( MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_POSITION, IDM_HELP_ABOUT, MENU_CLASS_SYSTEM_MENU_ABOUT_ITEM_TEXT );

			// Get argument list
			if( argumentList.Get() )
			{
				// Successfully got argument list

				// Process arguments
				argumentList.ProcessArguments( &OpenFileFunction );

			} // End of successfully got argument list

			// Show main window
			mainWindow.Show( nCmdShow );

			// Update main window
			mainWindow.Update();

			// Load combo box window
			g_comboBoxWindow.Load( COMBO_BOX_FILE_NAME );

			// Select first item on combo box window
			if( g_comboBoxWindow.SelectItem( 0 ) == 0 )
			{
				// Successfully selected first item on combo box window

				// Allocate string memory
				LPTSTR lpszFirstItemText = new char[ STRING_LENGTH + sizeof( char ) ];

				// Get first item text
				g_comboBoxWindow.GetItemText( 0, lpszFirstItemText );

				// Call selection changed function
				ComboBoxWindowSelectionChangedFunction( lpszFirstItemText );

				// Free string memory
				delete [] lpszFirstItemText;

			} // End of successfully selected first item on combo box window

			// Message loop
			while( message.Get() > 0 )
			{
				// Translate message
				message.Translate();

				// Dispatch message
				message.Dispatch();

			}; // End of message loop

		} // End of successfully created main window
		else
		{
			// Unable to create main window

			// Display error message
			MessageBox( NULL, WINDOW_CLASS_UNABLE_TO_CREATE_WINDOW_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

		} // End of unable to create main window

	} // End of successfully registered main window class
	else
	{
		// Unable to register main window class

		// Display error message
		MessageBox( NULL, WINDOW_CLASS_CLASS_UNABLE_TO_REGISTER_ERROR_MESSAGE, ERROR_MESSAGE_CAPTION, ( MB_OK | MB_ICONERROR ) );

	} // End of unable to register main window class

	return message;

} // End of function WinMain