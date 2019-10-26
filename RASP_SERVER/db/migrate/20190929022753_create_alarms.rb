class CreateAlarms < ActiveRecord::Migration[5.2]
  def change
    create_table :alarms do |t|
      t.integer :hour
      t.integer :min
      t.boolean :repeat_monday
      t.boolean :repeat_tuesday
      t.boolean :repeat_wednesday
      t.boolean :repeat_thursday
      t.boolean :repeat_friday
      t.boolean :repeat_saturday
      t.boolean :repeat_sunday

      t.timestamps
    end
  end
end
