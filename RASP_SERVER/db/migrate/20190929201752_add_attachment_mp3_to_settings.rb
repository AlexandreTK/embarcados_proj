class AddAttachmentMp3ToSettings < ActiveRecord::Migration[5.2]
  def self.up
    change_table :settings do |t|
      t.attachment :mp3
    end
  end

  def self.down
    remove_attachment :settings, :mp3
  end
end
