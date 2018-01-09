// 명령어		옵션	카피할 경로					카피 받을 경로
xcopy			/y		.\Engine\Headers\*.h					.\Reference\Headers\
xcopy			/y		.\Engine\Export\*.*						.\Reference\Headers\

xcopy			/y		.\Engine\System\Code\*.h				.\Reference\Headers\
xcopy			/y		.\Engine\System\bin\System.lib			.\Reference\Libraries\
xcopy			/y		.\Engine\System\bin\System.dll			.\Cilent\bin\
xcopy			/y		.\Engine\System\bin\System.dll			.\Toolbin\

xcopy			/y		.\Engine\Utility\Code\*.h				.\Reference\Headers\
xcopy			/y		.\Engine\Utility\bin\Utility.lib		.\Reference\Libraries\
xcopy			/y		.\Engine\Utility\bin\Utility.dll		.\Cilent\bin\
xcopy			/y		.\Engine\Utility\bin\Utility.dll		.\Toolbin\

xcopy			/y		.\Engine\Resources\Code\*.h				.\Reference\Headers\
xcopy			/y		.\Engine\Resources\bin\Resources.lib	.\Reference\Libraries\
xcopy			/y		.\Engine\Resources\bin\Resources.dll	.\Cilent\bin\
xcopy			/y		.\Engine\Resources\bin\Resources.dll	.\Toolbin\
