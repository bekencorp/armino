for %%f in (*.jpg) do (
	echo "%%f"
	..\bin\jpg420_2_jpg422.exe %%f out/%%~nf_jpg422.jpg
	del yuv420.bmp
)

pause