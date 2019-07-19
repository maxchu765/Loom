"{\
	'general':\
	{\
		'device_name':'Device',\
		'family':'Loom',\
		'instance_num':1,\
		'family_num':0\
	},\
	'components':[\
		{\
			'name':'Loom_Analog',\
			'parameters':'default'\
		},\
		{\
			'name':'Loom_Digital',\
			'parameters':'default'\
		},\
		{\
			'name':'Loom_Ethernet',\
			'parameters':[\
				'Ether1',\
				[<your-mac-address>],\
				[192,168,0,1]\
			]\
		},\
		{\
			'name':'Loom_NTP_Sync',\
			'parameters':[\
				'NTP1',\
				0,\
				0\
			]\
		},\
		{\
			'name':'Loom_GoogleSheets',\
			'parameters':[\
				'Goog',\
				0,\
				'/macros/s/<your-script-id>/exec',\
				'<your-sheet-id>',\
				'<your-tab-name>',\
				'<your-device-name>'\
			]\
		}\
	]\
}"