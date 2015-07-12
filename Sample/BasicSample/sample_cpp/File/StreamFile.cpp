
#include <Altseed.h>

// StreamFile ��p���āA�t�@�C������e�L�X�g��ǂݍ��ރT���v���ł��B

void StreamFile()
{
	// Altseed������������B
	asd::Engine::Initialize(asd::ToAString("StreamFile").c_str(), 640, 480, asd::EngineOption());

	// ���[�g�f�B���N�g����ǉ�����B
	asd::Engine::GetFile()->AddRootDirectory(asd::ToAString("Data").c_str());

	// �t�H���g�𐶐�����B
	auto font = asd::Engine::GetGraphics()->CreateFont(asd::ToAString("Font/Font1.aff").c_str());

	// �I�u�W�F�N�g�𐶐�����B
	auto obj = std::make_shared<asd::TextObject2D>();

	// �`��Ɏg���t�H���g��ݒ肷��
	obj->SetFont(font);

	// �`��ʒu���w�肷��
	obj->SetPosition(asd::Vector2DF(100, 100));

	// �t�@�C���I�u�W�F�N�g�쐬
	auto streamFile = asd::Engine::GetFile()->CreateStreamFile(asd::ToAString("Text/HelloWorld.txt").c_str());

	// �t�@�C���T�C�Y�擾
	const auto size = streamFile->GetSize();

	// �t�@�C���̓��e���o�b�t�@�֊i�[
	std::vector<uint8_t> buffer;
	streamFile->Read(buffer, size);

	// �`�悵�����e�L�X�g���t�@�C������ǂݎ��
	char text[32] = { 0 };
	memcpy(text, buffer.data(), size);

	// �`�悷�镶����̎w��
	obj->SetText(asd::ToAString(text).c_str());

	// �I�u�W�F�N�g�̃C���X�^���X���G���W���֒ǉ�����B
	asd::Engine::AddObject2D(obj);

	// Altseed�̃E�C���h�E�������Ă��Ȃ����m�F����B
	while (asd::Engine::DoEvents())
	{
		// Altseed���X�V����B
		asd::Engine::Update();
	}

	// Altseed���I������B
	asd::Engine::Terminate();
}