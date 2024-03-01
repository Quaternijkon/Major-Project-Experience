package org.excellence.yunnanemploymentbackend.entity;

import jakarta.persistence.ElementCollection;
import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@NoArgsConstructor
@Entity
public class Province {
    @Id private String userId;
    @ElementCollection private List<Integer> dataAwaitingReview;
    @ElementCollection private List<Integer> enterpriseInfoAwaitingReview;
    @ElementCollection private List<String> cities;
    @ElementCollection private List<Integer> releasedNotices;
    private Integer timeLimit;
}
