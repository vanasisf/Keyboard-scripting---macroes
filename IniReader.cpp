#include "StdAfx.h"
#include <stdio.h>

#define FILE_LINE_LENGTH	1024 * 65

void ConvertLineToParam( char *Line )
{
	if( Line[0] == '#' )
	{
//		printf("Read comment line : %s\n", Line );
		return;
	}
	char *ValueStart = strstr( Line, "=" );
	if( ValueStart != NULL && ValueStart != Line )
	{
		ValueStart++;
		char	StrValue[FILE_LINE_LENGTH];
		strcpy_s( StrValue, FILE_LINE_LENGTH, ValueStart );

		if( strstr( Line, "MOUSE_X_LIMIT_MIN" ) )
			GlobalStore.MouseXLimitMin = atoi( StrValue );
		else if( strstr( Line, "MOUSE_Y_LIMIT_MIN" ) )
			GlobalStore.MouseYLimitMin = atoi( StrValue );
		else if( strstr( Line, "MOUSE_X_LIMIT_MAX" ) )
			GlobalStore.MouseXLimitMax = atoi( StrValue );
		else if( strstr( Line, "MOUSE_Y_LIMIT_MAX" ) )
			GlobalStore.MouseYLimitMax = atoi( StrValue );
		else if( strstr( Line, "DOSBOX_WIDTH" ) )
			GlobalStore.DosBoxWidth = atoi( StrValue );
		else if( strstr( Line, "DOSBOX_HEIGHT" ) )
			GlobalStore.DosBoxHeight = atoi( StrValue );
		else if( strstr( Line, "WINDOW_TITLE_PART" ) )
			GlobalStore.ActiveWindowName = _strdup( StrValue );
		else if( strstr( Line, "WINDOW_SET_AUTOFOCUS" ) )
			GlobalStore.RefocusWindow = atoi( StrValue );
		else if( strstr( Line, "DEMOCRATIC_VOTE_WAIT" ) )
			GlobalStore.DemoctraticVoteWait = atoi( StrValue );
		//////////////////////////////////////
		else if( strstr( Line, "KEY_GROUP" ) )
		{
			memset( &GlobalStore.TempKeyStore, 0, sizeof( GlobalStore.TempKeyStore ) );
			GlobalStore.TempKeyStore.PlayerGroup = atoi( StrValue );
		}
		else if( strstr( Line, "KEY_IRC_TEXT" ) )
		{
			GlobalStore.TempKeyStore.IrcText = _strdup( StrValue );
			GlobalStore.TempKeyStore.IrcTextLen = strlen( GlobalStore.TempKeyStore.IrcText ) + 1;
		}
		else if( strstr( Line, "KEY_GAME_KEYCODE" ) )
			GlobalStore.TempKeyStore.StrokeCode = atoi( StrValue );
		else if( strstr( Line, "KEY_PRESSDOWN_TIME" ) )
			GlobalStore.TempKeyStore.StrokePushdownDelay = atoi( StrValue );
		else if( strstr( Line, "KEY_RELEASE_TIME" ) )
			GlobalStore.TempKeyStore.StrokeReleaseDelay = atoi( StrValue );
		else if( strstr( Line, "KEY_INTERVAL_TIME" ) )
			GlobalStore.TempKeyStore.PushInterval = atoi( StrValue );
		else if( strstr( Line, "KEY_GLOBAL_COOLDOWN" ) )
			GlobalStore.TempKeyStore.GlobalCooldown = atoi( StrValue );
		else if( strstr( Line, "KEY_MOUSE_CHANGE_X" ) )
			GlobalStore.TempKeyStore.MouseX = atoi( StrValue );
		else if( strstr( Line, "KEY_MOUSE_CHANGE_Y" ) )
			GlobalStore.TempKeyStore.MouseY = atoi( StrValue );
		else if( strstr( Line, "KEY_MOUSE_KEYCODE" ) )
			GlobalStore.TempKeyStore.MouseKey = atoi( StrValue );
		else if( strstr( Line, "KEY_MOUSE_MOVETYPE" ) )
			GlobalStore.TempKeyStore.MouseMoveType = atoi( StrValue );
		else if( strstr( Line, "KEY_MOUSE_FLAGS" ) )
		{
			GlobalStore.TempKeyStore.LastPushStamp = GetTickCount();
			GlobalStore.TempKeyStore.MouseFlags = atoi( StrValue );
			IrcGameKeyStore *Store = (IrcGameKeyStore *)malloc( sizeof( IrcGameKeyStore ) );
			memcpy( Store, &GlobalStore.TempKeyStore, sizeof( IrcGameKeyStore ) );
			GlobalStore.MonitoredKeys.push_back( Store );
//			printf("Loaded keymonitor : %s \n",GlobalStore.TempKeyStore.IrcText );
		}
		//////////////////////////////////////////////
		else if( strstr( Line, "MOUSE_BLACKOUT_X_START" ) )
		{
			memset( &GlobalStore.TempDenyBox, 0, sizeof( NoMouseClickBox ) );
			GlobalStore.TempDenyBox.XStart = atoi( StrValue );
		}
		else if( strstr( Line, "MOUSE_BLACKOUT_Y_START" ) )
			GlobalStore.TempDenyBox.YStart = atoi( StrValue );
		else if( strstr( Line, "MOUSE_BLACKOUT_X_END" ) )
			GlobalStore.TempDenyBox.XEnd = atoi( StrValue );
		else if( strstr( Line, "MOUSE_BLACKOUT_Y_END" ) )
			GlobalStore.TempDenyBox.YEnd = atoi( StrValue );
		else if( strstr( Line, "MOUSE_BLACKOUT_KEY" ) )
		{
			GlobalStore.TempDenyBox.MouseKey = atoi( StrValue );

			NoMouseClickBox *Store = (NoMouseClickBox *)malloc( sizeof( NoMouseClickBox ) );
			memcpy( Store, &GlobalStore.TempDenyBox, sizeof( NoMouseClickBox ) );

			GlobalStore.DenyMouseActionBoxes.push_back( Store );

			memset( &GlobalStore.TempDenyBox, 0, sizeof( NoMouseClickBox ) );
		}
		/////////////////
		else if( strstr( Line, "SEND_KEY_THREAD_MINWAIT" ) )
			GlobalStore.WorkerThreadLoopDuration = atoi( StrValue );
		else if( strstr( Line, "SEND_KEY_PAUSE_TOGGLE_KEYCODE" ) )
			GlobalStore.PauseToggleKeyCode = atoi( StrValue );
	}
}

int ReadLine( FILE *file, char *Store, int MaxLen )
{
	int StoreIndex = 0;
	Store[ 0 ] = 0;

	char ch = getc(file);
	if( ch == EOF )
		return 0;
    while( (ch != '\n') && (ch != '\r') && (ch != EOF) && StoreIndex < MaxLen ) 
	{
        Store[ StoreIndex ] = ch;
        StoreIndex++;
        ch = getc( file );
    }
    Store[ StoreIndex++ ] = '\0';
	return StoreIndex;
}

void LoadSettingsFromFile( )
{
	FILE *inf;
	errno_t err = fopen_s( &inf, "config.txt", "rt" );
	if( inf )
	{
		char buff[ FILE_LINE_LENGTH ];
		int ReadRet;

		//read while comments until EOF
		while( ReadRet = ReadLine( inf, buff, FILE_LINE_LENGTH ) )
			ConvertLineToParam( buff );

		fclose( inf );
	}
}