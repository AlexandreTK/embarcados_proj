class Setting < ApplicationRecord

	has_attached_file :mp3
	validates_attachment_content_type :mp3, content_type: ["audio/mp3", "audio/mpeg"]#, content_type: {content_type: ["audio/mpeg", "audio/mp3"]}#, file_name: { :matches => [/mp3\z/] }

end
