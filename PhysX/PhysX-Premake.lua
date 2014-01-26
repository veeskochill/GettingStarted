project "HelloPhysX"
   kind "ConsoleApp"
   language "C++"
   includedirs { "include" }
   files { "include/**.h", "src/**.cpp" }




    local physXSDK = 'C:/Program Files (x86)/NVIDIA Corporation/NVIDIA PhysX SDK/v3.2.1_win'
    if physXSDK == '' then
        assert(false, 'Could not find env value PHYSXSDK_WINDOWS')
    end

--    includedirs { physXSDK .. '/Include/PxTask' }
    includedirs { physXSDK .. '/Include/foundation' }
    includedirs { physXSDK .. '/Include' }
    includedirs { physXSDK .. '/Include/common' }
    includedirs { physXSDK .. '/Include/extensions' }

    links { physXSDK .. '/lib/win32/PhysX3CHECKED_x86' }
--    links { physXSDK .. '/lib/win32/PhysX3CookingCHECKED_x86' }
--    links { physXSDK .. '/lib/win32/PxTaskCHECKED' }
    links { physXSDK .. '/lib/win32/PhysX3ExtensionsCHECKED' }
    links { physXSDK .. '/lib/win32/PhysX3CommonCHECKED_x86' }
--    links { physXSDK .. '/lib/win32/PhysXVisualDebuggerSDKCHECKED' }

    local apexSDK = 'C:/Program Files (x86)/NVIDIA Corporation/APEX SDK/1.2/PhysX_3.2'
    if apexSDK == '' then
        assert(false, 'Could not find env value APEXSDK_WINDOWS')
    end
 --   includedirs { apexSDK .. '/framework/public' }
 --   includedirs { apexSDK .. '/public' }
 --   includedirs { apexSDK .. '/shared/general/shared' }
 --   includedirs { apexSDK .. '/shared/external/include' }
 --   includedirs { apexSDK .. '/framework/public/PhysX3' }
 --   includedirs { apexSDK .. '/NxParameterized/public' }
 --   includedirs { apexSDK .. '/module/destructible/public' }

  --  links { apexSDK .. '/lib/vc10win32-PhysX_3.2/ApexFrameworkCHECKED_x86' }

   configuration "Debug"
      flags { "Symbols" }
      defines { "_DEBUG", "WIN32" }