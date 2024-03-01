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
public class City {
    @Id private String userId;
    @ElementCollection private List<Integer> dataAwaitingReview;
    @ElementCollection private List<String> enterprises;
    private String locatedProvince; // 所在省的UserId
}
