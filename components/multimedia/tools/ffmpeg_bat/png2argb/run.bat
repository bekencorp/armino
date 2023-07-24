for %%f in (*.png) do (
	del %%~nf_output_argb.rgb
	..\bin\ffmpeg -i %%f -pix_fmt rgba %%~nf_output_argb.rgb
)

pause