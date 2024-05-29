#include <cstdio>
#include <cstdlib>

#include <zip.h>

int main(int argc, char** argv)
{
	int error = 0;
	zip_t* z = zip_open(argv[1], ZIP_RDONLY, &error);
	
	if(!z)
	{
		fprintf(stderr, "Cannot open \"%s\": error=%d.\n", argv[1], error);
		return 1;
	}
	
	const zip_int64_t num = zip_get_num_entries(z, 0);
	
	for(int n=0; n<num; ++n)
	{
		zip_stat_t zs;
		zip_stat_init(&zs);
		
		const int ret = zip_stat_index(z, n, 0, &zs);
		if(ret!=0)
		{
			fprintf(stderr, "Cannot stat entry #%d. \n", n);
			continue;
		}
		
		printf("#%d: ", n);
		
		if(zs.valid & ZIP_STAT_NAME)
		{
			printf("\"%s\" ", zs.name);
		}
		
		if(zs.valid & ZIP_STAT_SIZE)
		{
			printf("%llu bytes. ", (unsigned long long)zs.size );
		}

		if(zs.valid & ZIP_STAT_COMP_SIZE)
		{
			printf("comp %llu bytes. ", (unsigned long long)zs.comp_size );
		}
		
		printf("\n"); fflush(stdout);
	}
	
	zip_close(z);
}
