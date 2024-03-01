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
public class Enterprise {
    @Id private String userId;
    private String locatedCity;
    private Integer enterpriseInfoId;
    @ElementCollection private List<Integer> reportedData;
    @ElementCollection private List<Integer> unreadNotices;
    private Boolean isRegistered;
}
