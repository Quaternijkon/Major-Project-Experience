package org.excellence.yunnanemploymentbackend.entity;

import jakarta.persistence.Entity;
import jakarta.persistence.GeneratedValue;
import jakarta.persistence.GenerationType;
import jakarta.persistence.Id;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@Entity
public class Notice {
    @Id @GeneratedValue(strategy = GenerationType.AUTO)
    private Integer noticeId;
    private String title;
    private String content; // 通知内容
    private String date; // 发布日期
}
