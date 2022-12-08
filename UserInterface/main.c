#include <fts.h>
#include "ui.h"

int rmDir(const char *dir)
{
	int toReturnValue = 0;
	FTSENT *currentFiles;
	char *filesToRm[] = {(char *)dir, NULL};
	FTS *ftsp
		= fts_open(filesToRm, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
	if (!ftsp)
	{
		toReturnValue = -1;
		goto endd;
	}
	while ((currentFiles = fts_read(ftsp)))
	{
		switch (currentFiles->fts_info)
		{
			case FTS_NS:
			case FTS_DNR:
			case FTS_ERR:
				toReturnValue = -1;
				break;
			case FTS_DOT:
			case FTS_NSOK:
			case FTS_DC:
				break;
			case FTS_D:
				break;
			case FTS_SL:
			case FTS_SLNONE:
			case FTS_DEFAULT:
			case FTS_DP:
			case FTS_F:
				if (remove(currentFiles->fts_accpath) < 0)
				{
					fprintf(stderr, "%s: Failed to remove: %s\n",
						currentFiles->fts_path,
						strerror(currentFiles->fts_errno));
					toReturnValue = -1;
				}
				break;
		}
	}

endd:
	if (ftsp)
	{
		fts_close(ftsp);
	}
	return toReturnValue;
}

int main(int argc, char *argv[])
{
	// init Gtk
	gtk_init(&argc, &argv);
	// init trigonometric tables
	initTrig();
	// init UserInterface
	uiLaunch();
	// remove tmp folder
	rmDir("tmpImg/");
	return 0;
}