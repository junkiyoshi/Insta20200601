#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofDisableArbTex();

	this->cap.open("D:\\MP4\\video2.mp4");
	this->cap_size = cv::Size(640, 360);

	this->rect_size = 40;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_images.push_back(move(image));
			this->cv_rects.push_back(rect);
			this->rect_frames.push_back(frame);
		}
	}

	this->number_of_frames = 25 * 27;
	for (int i = 0; i < this->number_of_frames; i++) {

		cv::Mat src, tmp;
		this->cap >> src;
		if (src.empty()) {

			return;
		}

		cv::resize(src, tmp, this->cap_size);
		cv::cvtColor(tmp, tmp, cv::COLOR_BGR2RGB);

		this->frame_list.push_back(tmp);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofTranslate(this->cap_size.width * -0.5, this->cap_size.height * -0.5, (25 * 27) * -0.5);

	for (int k = 0; k < 3; k++) {

		float noise_seed = ofRandom(1000);
		for (int i = 0; i < this->rect_frames.size(); i++) {

			int frame_index = ofMap(ofNoise(noise_seed, this->cv_rects[i].x * 0.00035, this->cv_rects[i].y * 0.00035, ofGetFrameNum() * 0.003), 0, 1, 0, 25 * 27);

			cv::Mat tmp_box_image(this->frame_list[frame_index], this->cv_rects[i]);
			tmp_box_image.copyTo(this->rect_frames[i]);

			this->rect_images[i]->update();
			this->rect_images[i]->getTexture().bind();
			ofSetColor(255);
			ofFill();
			ofDrawBox(glm::vec3(this->cv_rects[i].x, this->cap_size.height - this->cv_rects[i].y, frame_index * 2), this->rect_size, this->rect_size, 5);
			this->rect_images[i]->unbind();

			ofSetColor(39);
			ofNoFill();
			ofDrawBox(glm::vec3(this->cv_rects[i].x, this->cap_size.height - this->cv_rects[i].y, frame_index * 2), this->rect_size, this->rect_size, 5);

			ofPopMatrix();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}