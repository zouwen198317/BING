#include "stdafx.h"
#include "ValStructVec.h"
#include "ObjectnessTest.h"
#include "Dataset.h"
#include "CnnFace.h"


void RunFaceProposal(int W, int NSS, int numPerSz);

void cnnFaceDetectionTest();

static int create_directory(const char *directory);
static int get_dir_from_filename(const char *file_name, char *dir);
static int create_directory_from_filename(const char *file_name);


void main(int argc, char* argv[])
{
	//RunFaceProposal(8, 2, 150);   // BING test

	cnnFaceDetectionTest();			// cnn face detection
}

// BING test code
void RunFaceProposal(int W, int NSS, int numPerSz)
{
	// configuration
	string imgPath  = "Z:\\User\\wuxiang\\data\\face_detection\\FDDB\\originalPics";
	string listPath = "Z:\\User\\wuxiang\\data\\face_detection\\FDDB\\FDDB_list.txt";
	string frPath = "Z:\\Temp\\CNN_Face_Detection\\fr\\man";
	string modelPath = "D:\\svn\\Algorithm\\wuxiang\\Code\\C\\BING\\model\\ObjNessB2W8MAXBGR.wS1";

	string savePath = "D:\\BING\\fr";

	string saveImgPath = "D:\\BING\\img";

	vector<vector<Vec4i>> frsImgs;
	char fr[_MAX_PATH];

	// load image
	DataSet dataSet(imgPath, listPath, frPath);
	dataSet.loadAnnotations();

	// predict
	ObjectnessTest objectNessTest(dataSet, modelPath, W, NSS);
	objectNessTest.loadTrainedModel(modelPath);
	objectNessTest.getFaceProposalsForImgsFast(frsImgs, numPerSz);

	// save 
	for (int i = 0; i < frsImgs.size(); i++)
	{
		//cout << dataSet.imgPathFr[i].c_str() << "is saving" <<endl;
		sprintf(fr, "%s/%s", savePath.c_str(), dataSet.imgPathFr[i].c_str());
		create_directory_from_filename(fr);
		FILE *fp = fopen(fr, "wt");
		fprintf(fp, "%d\n", frsImgs[i].size());
		for (int j = 0; j < frsImgs[i].size(); j++)
		{
			Vec4i box = frsImgs[i][j];
			fprintf(fp, "%d\t%d\t%d\t%d\n", box[0], box[1], box[2], box[3]);
		}
		fclose(fp);
		string imgSavePath = saveImgPath + "\\" + dataSet.imgPathName[i] + "_Match.jpg";
		create_directory_from_filename(imgSavePath.c_str());
		objectNessTest.illuTestReults(imgPath + "\\" +dataSet.imgPathName[i], imgSavePath, dataSet.imgFr[i], frsImgs[i]);
	}
}

// cnn face detection test
void cnnFaceDetectionTest()
{
	// configuration
	string imgPath  = "Z:\\User\\wuxiang\\data\\face_detection\\FDDB\\originalPics";
	string listPath = "Z:\\User\\wuxiang\\data\\face_detection\\FDDB\\test.txt";
	string frPath = "Z:\\Temp\\CNN_Face_Detection\\fr\\man";
	string bingModelPath = "D:\\svn\\Algorithm\\wuxiang\\Code\\C\\BING\\model\\ObjNessB2W8MAXBGR.wS1";
	string cnnModelPath = "D:\\svn\\Algorithm\\wuxiang\\Code\\C\\BING\\model\\24_detection.bin";

	const int W = 8, NSS = 2, numPerSz = 150;

	vector<Vec4i> boxTestStageI;
	vector<Vec4i> boxTestStageII;
	char fr[_MAX_PATH];

	// load image
	DataSet dataSet(imgPath, listPath, frPath);
	dataSet.loadAnnotations();

	// predict
	ObjectnessTest objectNessTest(dataSet, bingModelPath, W, NSS);
	objectNessTest.loadTrainedModel(bingModelPath);
	for(int i = 0; i < dataSet.testNum; i++)
	{
		// face detection Stage I: get face region proposal
		boxTestStageI.clear();
		printf("Process %d images..\n", i);
		Mat img = imread(dataSet.imgPath + "\\" + dataSet.imgPathName[i]);
		boxTestStageI.reserve(10000);
		objectNessTest.getFaceProposaksForPerImgFast(img, boxTestStageI, numPerSz);

		// 
	}
}

static int create_directory(const char *directory)
{
	int i;
	  int len;
	  char dir[_MAX_PATH], temp_dir[_MAX_PATH];

	  memcpy(temp_dir, directory, _MAX_PATH);

	  len = (int)strlen(temp_dir);
	  for(i=0; i<len; i++)
	  {
		if(temp_dir[i] == '\\')
		  temp_dir[i] = '/';
	  }
	  if(temp_dir[len-1] != '/')
	  {
		temp_dir[len] = '/';
		temp_dir[len+1] = 0;
		len++;
	  }
	  memset(dir, 0, _MAX_PATH);
	  for(i=0; i<len; i++)
	  {
		dir[i] = temp_dir[i];
		if(temp_dir[i] == '/')
		{
		  if(i > 0)
		  {
			if(temp_dir[i-1] == ':')
			  continue;
			else
			{
				if(_access(dir, 0) == 0)
				continue;
			  else /* create it */
			  {
				if(_mkdir(dir) != 0)
				  return -1;
			  }
			}
		  }
		}
	  }

	  return 0;
}

static int get_dir_from_filename(const char *file_name, char *dir)
{
	  int len;
	  int i;

	  len = (int) strlen(file_name);
	  for(i=len-1; i>=0; i--)
	  {
			if(file_name[i] == '\\' || file_name[i] == '/')
			{
				break;
			}
	  }

	  strcpy(dir, file_name);
	  dir[i+1] = 0;

	  return 0;
}

static int create_directory_from_filename(const char *file_name)
{
	char dir[_MAX_PATH];
	get_dir_from_filename(file_name, dir);
	create_directory(dir);
	return 0;
}

